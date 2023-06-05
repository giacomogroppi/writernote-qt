#include "fromimage.h"
#include "core/WListFast.h"
#include "dataread/readlistarray.h"
#include "core/WZipWriter.h"

fromimage::load_res_img fromimage::save_img(WZipWriter          &writer,
                                    const WListFast<WString>   &pathPdf) const
{
    fromimage::load_res_img res;

    for(const auto &str : std::as_const(pathPdf)){
        res = this->save_img(writer, str);

        if(res != fromimage::load_res_img::ok)
            return res;
    }

    return fromimage::load_res_img::ok;
}

fromimage::load_res_img fromimage::save_img(WZipWriter              &writer,
                                    const WString           &path) const
{
    WByteArray img_in_byte;
    WPixmap img;

    if(get_img_bytearray(img_in_byte, path) != load_res_img::ok){
        return load_res_img::error;
    }

    if(!img.loadFromData(img_in_byte, "PNG"))
        return load_res_img::err_image_not_valid;

    if(writer.write(img_in_byte.constData(), img_in_byte.size(), fromimage::getName_img(this->length_img()).constData()))
        return load_res_img::error;

    return load_res_img::ok;
}

fromimage::load_res_img fromimage::save_metadata_img(WZipWriterSingle &writer)
{
    for(const auto &img : std::as_const(m_img))
    {
        const double val[] = {
                img.i.x(),
                img.i.y(),
                img.f.x(),
                img.f.y()
        };

        writer.write(val, sizeof(val));

        static_assert(sizeof(val) == sizeof(double) * 4);
    }

    return load_res_img::ok;
}

size_t fromimage::get_size_file_img() const
{
    const size_t s = sizeof(double) * 4 * this->length_img();
    return s;
}

fromimage::load_res_img fromimage::get_img_bytearray(WByteArray &arr, const WString &path)
{
    WPixmap img(path);

    if(img.isNull()){
        return load_res_img::err_image_not_valid;
    }

    if(!img.save_and_size(arr))
        return load_res_img::error;

    return load_res_img::ok;
}

fromimage::load_res_img fromimage::load_metadata_img(WZipReaderSingle &reader, int len)
{
    int i;
    double val[4];
    struct immagine_s img;

    static_assert(sizeof(val) == sizeof(double) * 4);

    for(i = 0; i < len; ++i){
        if(reader.read_by_size(val, sizeof(val))){
            return load_res_img::error;
        }

        img.i = PointF(val[0], val[1]);
        img.f = PointF(val[2], val[3]);

        m_img.append(img);
    }

    return load_res_img::ok;
}

fromimage::load_res_img fromimage::load_img(WZipReaderSingle &reader, int len)
{
    WListFast<WByteArray> arr;
    WListFast<WString> name_list;
    unsigned char res;

    this->m_img.clear();

    name_list = this->get_name_img();

    if(this->load_metadata_img(reader, len) != load_res_img::ok)
        return load_res_img::err_meta_data;

    res = readListArray::read(name_list, *(reader.get_zip()), arr, false);
    if(res != OK){
        return fromimage::load_res_img::error;
    }

    return fromimage::load_multiple_img(arr);
}

fromimage::load_res_img fromimage::load_single_img(const WByteArray &arr,
                                           struct immagine_s &img)
{
    if(!img.immagini.loadFromData(arr, "PNG"))
        return load_res_img::error;

    if(img.immagini.isNull())
        return load_res_img::err_image_not_valid;

    return load_res_img::ok;
}

/*
 * in m_img sono già presenti tutte le immagini con i metadati
 * dobbiamo solo cambiare l'immagine e l'array
*/
fromimage::load_res_img fromimage::load_multiple_img(const WListFast<WByteArray> &arr)
{
    uint i, len;
    fromimage::load_res_img res;
    struct immagine_s img;

    len = arr.size();
    for(i=0; i<len; ++i){
        res = fromimage::load_single_img(arr.at(i), m_img.operator[](i));
        if(res != fromimage::load_res_img::ok)
            return res;
    }

    return fromimage::load_res_img::ok;
}

WListFast<WString> fromimage::get_name_img()
{
    int i;
    WListFast<WString> list;

    for(i = 0; i < length_img(); ++i){
        list.append(fromimage::getName_img(i));
    }

    return list;
}

unsigned fromimage::insert_image(   const WString &pos,
                                    const PointSettable *point,
                                    struct immagine_s &img)
{
    WString res;
    W_ASSERT(pos.size());

    img.immagini = std::move(WImage(res));

    if (point) {
        img.i = *point;
        img.f = *point + PointF(DELTA_POINT, DELTA_POINT);
    } else {
        img.i = PointF(0, 0);
        img.f = PointF(DELTA_POINT, DELTA_POINT);
    }

    return OK;
}

/*
 * add image from position
*/
int fromimage::addImage(    const WString &pos,
                            const PointSettable *point,
                            const WString &path_writernote)
{
    struct immagine_s img;
    WZipWriter writer;

    if(writer.init(path_writernote.toUtf8().constData()))
        return -1;

    if(insert_image(pos, point, img) != OK)
        return -2;

    this->m_img.append(img);

    if(this->save_img(writer, pos) != fromimage::ok)
        return -3;

    return 0;
}
