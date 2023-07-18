#include "fromimage.h"
#include "core/WListFast.h"
#include "dataread/readlistarray.h"
#include "FileContainer/WZipWriter.h"

ImageContainerDrawable::load_res_img ImageContainerDrawable::saveImage(WZipWriter          &file,
                                                                       const WListFast<WString>   &path) const
{
    ImageContainerDrawable::load_res_img res;

    for(const auto &str : std::as_const(path)){
        res = this->saveImage(file, str);

        if(res != ImageContainerDrawable::load_res_img::ok)
            return res;
    }

    return ImageContainerDrawable::load_res_img::ok;
}

ImageContainerDrawable::load_res_img ImageContainerDrawable::saveImage(WZipWriter              &file,
                                                                       const WString           &path) const
{
    WByteArray img_in_byte;
    WPixmap img;

    if(getImageRawData(img_in_byte, path) != load_res_img::ok){
        return load_res_img::error;
    }

    if(!img.loadFromData(img_in_byte, "PNG"))
        return load_res_img::err_image_not_valid;

    if(file.write(img_in_byte.constData(), img_in_byte.size(), ImageContainerDrawable::getName_img(this->lengthImage()).constData()))
        return load_res_img::error;

    return load_res_img::ok;
}

ImageContainerDrawable::load_res_img ImageContainerDrawable::saveMetadataImage(WZipWriterSingle &writer)
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

size_t ImageContainerDrawable::getSizeFileImage() const
{
    const size_t s = sizeof(double) * 4 * this->lengthImage();
    return s;
}

ImageContainerDrawable::load_res_img ImageContainerDrawable::getImageRawData(WByteArray &arr, const WString &path)
{
    WPixmap img(path);

    if(img.isNull()){
        return load_res_img::err_image_not_valid;
    }

    if(!img.save_and_size(arr))
        return load_res_img::error;

    return load_res_img::ok;
}

ImageContainerDrawable::load_res_img ImageContainerDrawable::loadMetadataImage(WZipReaderSingle &reader, int len)
{
    int i;
    double val[4];
    ImageDrawable img;

    static_assert(sizeof(val) == sizeof(double) * 4);

    for(i = 0; i < len; ++i){
        if(reader.readBySize(val, sizeof(val))){
            return load_res_img::error;
        }

        img.i = PointF(val[0], val[1]);
        img.f = PointF(val[2], val[3]);

        m_img.append(img);
    }

    return load_res_img::ok;
}

ImageContainerDrawable::load_res_img ImageContainerDrawable::loadImage(WZipReaderSingle &zip, int len)
{
    WListFast<WByteArray> arr;
    WListFast<WString> name_list;
    unsigned char res;

    this->m_img.clear();

    name_list = this->get_name_img();

    if(this->loadMetadataImage(zip, len) != load_res_img::ok)
        return load_res_img::err_meta_data;

    res = readListArray::read(name_list, *(zip.getZip()), arr, false);
    if(res != OK){
        return ImageContainerDrawable::load_res_img::error;
    }

    return ImageContainerDrawable::loadMultipleImage(arr);
}

ImageContainerDrawable::load_res_img ImageContainerDrawable::loadSingleImage(const WByteArray &arr,
                                                                             ImageDrawable &img)
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
ImageContainerDrawable::load_res_img ImageContainerDrawable::loadMultipleImage(const WListFast<WByteArray> &arr)
{
    int i, len;
    ImageContainerDrawable::load_res_img res;
    ImageDrawable img;

    len = arr.size();
    for (i = 0; i < len; i++){
        res = ImageContainerDrawable::loadSingleImage(arr.at(i), m_img.operator[](i));
        if(res != ImageContainerDrawable::load_res_img::ok)
            return res;
    }

    return ImageContainerDrawable::load_res_img::ok;
}

WListFast<WString> ImageContainerDrawable::get_name_img()
{
    int i;
    WListFast<WString> list;

    for(i = 0; i < lengthImage(); ++i){
        list.append(ImageContainerDrawable::getName_img(i));
    }

    return list;
}

unsigned ImageContainerDrawable::insert_image(const WString &pos,
                                              const PointSettable *point,
                                              ImageDrawable &img)
{
    WString res;
    W_ASSERT(pos.size());

    img.immagini = WImage(res);

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
int ImageContainerDrawable::addImage(const WString &pos,
                                     const PointSettable *point,
                                     const WString &path_writernote)
{
    ImageDrawable img;
    WZipWriter writer;

    if(writer.init(path_writernote.toUtf8().constData()))
        return -1;

    if(insert_image(pos, point, img) != OK)
        return -2;

    this->m_img.append(img);

    if(this->saveImage(writer, pos) != ImageContainerDrawable::ok)
        return -3;

    return 0;
}

ImageContainerDrawable &ImageContainerDrawable::operator=(const ImageContainerDrawable &other) noexcept
{
    if (this == &other)
        return *this;

    this->m_img = other.m_img;

    return *this;
}

ImageContainerDrawable &ImageContainerDrawable::operator=(ImageContainerDrawable &&other) noexcept
{
    if (this == &other)
        return *this;

    m_img = std::move (other.m_img);

    return *this;
}

ImageContainerDrawable::ImageContainerDrawable(const ImageContainerDrawable &other) noexcept
    : m_img(other.m_img)
{

}

ImageContainerDrawable::ImageContainerDrawable(ImageContainerDrawable &&other) noexcept
    : m_img (std::move(other.m_img))
{

}

auto ImageContainerDrawable::load(const VersionFileController &versionController,
                                  ReadableAbstract &readable) -> WPair<int, ImageContainerDrawable>
{
    ImageContainerDrawable result;

    auto [res, d] = WListFast<ImageDrawable>::load(versionController, readable);
    if (res < 0)
        return {-1, result};

    result.m_img = std::move(d);

    return {0, result};
}

auto ImageContainerDrawable::write(WritableAbstract &writable, const ImageContainerDrawable &source) -> int
{
    if (WListFast<ImageDrawable>::write(writable, source.m_img) < 0)
        return -1;
    return 0;
}
