#include "page_file.h"
#include "core/WReadZip.h"
#include "datawrite/source_read_ext.h"
#include "touch/datastruct/page.h"
#include "core/WZipWriterMulti.h"
#include <QImage>

#ifdef ALL_VERSION
int page_file::load_ver_0(page &_page, WZipReaderSingle &reader)
{
    constexpr int ver_stroke = 0;
    int i, k, len_stroke, err = OK;

    // user stroke
    if(reader.read_object(len_stroke) < 0)
        return ERROR;

    for(i = 0; i < len_stroke; i++){
        _page._stroke.append(stroke());
        stroke &ref = _page._stroke.last();
        err = ref.load(reader, ver_stroke);

        if(unlikely(err != OK))
            return err;
    }

    // writernote stroke [for page style]
    if(reader.read_object(len_stroke) < 0)
        return ERROR;

    for(i = 0; i < len_stroke; i++){
        stroke __tmp;
        const int res = __tmp.load(reader, ver_stroke);
        if(unlikely(res != OK))
            return res;

        if(unlikely(res == PAGE_POINT)){
            cint len = __tmp.length();
            for(k = 0; k < len; k ++){
                _page._stroke_writernote.append(__tmp.at(k), __tmp.getPressure(k));
            }
        }
    }

    // remove empty stroke
    for(int i = _page.lengthStroke() - 1; i >= 0; i--){
        if(_page.atStroke(i).length() == 0){
            _page._stroke.removeAt(i);
        }
    }

    return OK;
}

int page_file::load_ver_1(page &_page, WZipReaderSingle &reader)
{
    constexpr int ver_stroke = 1;
    int i, len_stroke, err = OK;

    if(reader.read_object(len_stroke) < 0)
        return ERROR;

    for(i = 0; i < len_stroke; i++){
        _page._stroke.append(stroke());
        stroke &ref = _page._stroke.last();
        err = ref.load(reader, ver_stroke);

        if(unlikely(err != OK))
            return err;
    }

    _page._stroke_writernote.load(reader, ver_stroke);
    if(_page._stroke_writernote.length() && _page._stroke_writernote.getPressure() > 10){
        _page._stroke_writernote.__setPressureFirstPoint(1.5);
    }

    return err;
}

#endif // ALL_VERSION

int page_file::load_ver_2(page &_page, WZipReaderSingle &reader)
{
    constexpr int ver_stroke = 2;
    int err, i, len_stroke;
    QByteArray arr;
    size_t size;

    if(reader.read_object(len_stroke) < 0)
        return ERROR;

    for(i = 0; i < len_stroke; i++){
        _page._stroke.append(stroke());
        stroke &ref = _page._stroke.last();

        err = ref.load(reader, ver_stroke);

        if(unlikely(err != OK))
            return err;
    }

    err = _page._stroke_writernote.load(reader, ver_stroke);

    if(unlikely(err != OK))
        return err;

    if(reader.read_object(size) < 0)
        return ERROR;

    char _raw[size];
    if(reader.read_by_size(_raw, size) < 0)
        return ERROR;
    arr = QByteArray::fromRawData((cchar *)_raw, size);

    _page._imgDraw.loadFromData(arr, "PNG");

    return OK;
}

int page_file::load(page &_page, int ver_stroke, WZipReaderSingle &reader)
{
    switch (ver_stroke) {
#ifdef ALL_VERSION

    case 0:
        return page_file::load_ver_0(_page, reader);
    case 1:
        return page_file::load_ver_1(_page, reader);
#else
    case 0 ... 1: return ERROR_VERSION;
#endif // ALL_VERSION
    case 2:
        return page_file::load_ver_2(_page, reader);
    default:
        return ERROR_VERSION;
    }
}

size_t page_file::size_in_file(const page &_page, cbool saveImg)
{
    size_t s = 0;

    s += sizeof(int);       // len stroke

    for(const auto &ref: qAsConst(_page._stroke)){
        s += ref.getSizeInFile();
    }

    s += _page._stroke_writernote.getSizeInFile();

    s += sizeof(size_t);    // size img
    if(likely(saveImg)){
        s += _page._imgDraw.get_size_in_file();
    }

    return s;
}

int page_file::save(const page *_page, WZipWriterSingle &writer, cbool saveImg)
{
    int i, err = OK;
    const int len = _page->_stroke.length();
    size_t size;
    QByteArray arr;

    W_ASSERT(_page);

    static_assert(sizeof(len) == sizeof(int));
    static_assert(sizeof(size) == sizeof(size_t));

    /* stroke len */
    writer.write_object(len);

    for(i = 0; i < len; i++){
        err = _page->atStroke(i).save(writer);
        if(unlikely(err != OK))
            return err;
    }

    err = _page->_stroke_writernote.save(writer);
    if(unlikely(err != OK))
        return err;

    {
        if(unlikely(saveImg)){
            (void)_page->_imgDraw.save_and_size(arr);
        }

        size = arr.size();

        writer.write_object(size);
        if(likely(size))
            writer.write(arr.constData(), size);
    }

    return OK;
}
