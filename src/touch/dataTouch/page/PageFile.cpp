#include "PageFile.h"
#include "core/WReadZip.h"
#include "datawrite/source_read_ext.h"
#include "Page.h"
#include "core/WZipWriterMulti.h"
#include <QImage>
#include "utils/common_error_definition.h"
#include "touch/dataTouch/stroke/StrokeNormal.h"

int PageFile::load_ver_0(Page &, WZipReaderSingle &)
{
    return ERROR_VERSION_NEW;
}

int PageFile::load_ver_1(Page &_page, WZipReaderSingle &reader)
{
    constexpr int ver_stroke = 1;
    int i, len_stroke, err = OK;

    if(reader.read_object(len_stroke) < 0)
        return ERROR;

    for(i = 0; i < len_stroke; i++){
        int ok;
        std::shared_ptr<Stroke> res = Stroke::load(reader, ver_stroke, &ok);

        if(un(ok != OK))
            return ok;

        if(un(err != OK))
            return err;

        _page._stroke.append(res);
    }

    _page._stroke_writernote.load(reader, ver_stroke);

    return err;
}

int PageFile::load_ver_2(Page &_page, WZipReaderSingle &reader)
{
    constexpr int ver_stroke = 2;
    int err, i, len_stroke;
    QByteArray arr;
    size_t size;

    if(reader.read_object(len_stroke) < 0)
        return ERROR;

    for(i = 0; i < len_stroke; i++){
        int ok;
        std::shared_ptr<Stroke> ref = Stroke::load(reader, ver_stroke, &ok);

        if (ok != OK)
            return ok;

        _page._stroke.append(ref);
    }

    err = _page._stroke_writernote.load(reader, ver_stroke);

    if(un(err != OK))
        return err;

    if(reader.read_object(size) < 0)
        return ERROR;

    if(size){
        char _raw[size];
        if(reader.read_by_size(_raw, size) < 0)
            return ERROR;
        arr = QByteArray::fromRawData((cchar *)_raw, size);

        _page._imgDraw.loadFromData(arr, "PNG");
    }else{
        _page._imgDraw = WImage();
    }

    return OK;
}

int PageFile::load(Page &_page, int ver_stroke, WZipReaderSingle &reader)
{
    switch (ver_stroke) {
    case 0:
        return PageFile::load_ver_0(_page, reader);
    case 1:
        return PageFile::load_ver_1(_page, reader);

    case 2:
        return PageFile::load_ver_2(_page, reader);
    default:
        return ERROR_VERSION;
    }
}

size_t PageFile::size_in_file(const Page &_page, cbool saveImg)
{
    size_t s = 0;

    s += sizeof(int);       // len stroke

    for(const auto &ref: qAsConst(_page._stroke)){
        s += ref->getSizeInFile();
    }

    s += _page._stroke_writernote.getSizeInFile();

    s += sizeof(size_t);    // size img
    if(likely(saveImg)){
        s += _page._imgDraw.get_size_in_file();
    }

    return s;
}

int PageFile::save(const Page *_page, WZipWriterSingle &writer, cbool saveImg)
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
        if(un(err != OK))
            return err;
    }

    err = _page->_stroke_writernote.save(writer);
    if(un(err != OK))
        return err;

    {
        if(un(saveImg)){
            (void)_page->_imgDraw.save_and_size(arr);
        }

        size = arr.size();

        writer.write_object(size);
        if(likely(size))
            writer.write(arr.constData(), size);
    }

    return OK;
}
