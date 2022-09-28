#include "fromimage.h"
#include "stdlib.h"
#include <QBuffer>
#include <QByteArray>
#include <QList>
#include <QFileDialog>
#include "zip.h"
#include "dataread/readlistarray.h"
#include "datawrite/source_read_ext.h"
#include "currenttitle/document.h"
#include "dataread/xmlstruct.h"
#include "core/WZipWriter.h"

#define FIRST_SOURCE_READ(x, y, z) ARGUMENT(x,y,z)return ERROR;

fromimage::load_res fromimage::save(WZipWriter          &writer,
                                    const QStringList   &pathPdf) const
{
    fromimage::load_res res;
    uint i, len;

    doc->count_img = pathPdf.length();
    len = doc->count_img;

    for(i=0; i<len; ++i){
        res = this->save(writer, pathPdf.at(i));

        if(res != fromimage::load_res::ok)
            return res;
    }
    return fromimage::load_res::ok;
}

fromimage::load_res fromimage::save(WZipWriter              &writer,
                                    const QString           &path) const
{
    QByteArray img_in_byte;
    QImage img;

    if(get_img_bytearray(img_in_byte, path) != load_res::ok){
        return load_res::error;
    }

    if(!img.loadFromData(img_in_byte, "PNG"))
        return load_res::err_image_not_valid;

    if(writer.write(img_in_byte.constData(), img_in_byte.size(), fromimage::getName(doc->count_img)))
        return load_res::error;

    return load_res::ok;
}

fromimage::load_res fromimage::save_metadata(WZipWriterSingle &writer)
{
    unsigned i;
    double val[4];

    for(i = 0; i < doc->count_img; i++){
        const struct immagine_s &img = m_img.at(i);

        val[0] = img.i.x();
        val[1] = img.i.y();
        val[2] = img.f.x();
        val[3] = img.f.y();

        writer.write(val, sizeof(val));

        static_assert(sizeof(val) == sizeof(double) * 4);
    }

    return load_res::ok;
}

size_t fromimage::get_size_file() const
{
    const size_t s = sizeof(double) * 4 * doc->count_img;
    return s;
}

fromimage::load_res fromimage::get_img_bytearray(QByteArray &arr, const QString &path) const
{
    QImage img(path);
    QBuffer buffer(&arr);

    arr.clear();

    if(img.isNull()){
        return load_res::err_image_not_valid;
    }

    if(!buffer.open(QIODevice::WriteOnly)){
        return load_res::error;
    }

    buffer.seek(0);

    if(!img.save(&buffer, "PNG")){
        return load_res::error;
    }

    return load_res::ok;
}

fromimage::load_res fromimage::load_metadata(WZipReaderSingle &reader)
{
    uint i;
    double val[4];
    struct immagine_s img;

    static_assert(sizeof(val) == sizeof(double) * 4);

    for(i = 0; i < this->doc->count_img; ++i){
        if(reader.read_by_size(val, sizeof(val))){
            return load_res::error;
        }

        img.i = QPointF(val[0], val[1]);
        img.f = QPointF(val[2], val[3]);

        m_img.append(img);
    }

    return load_res::ok;
}

fromimage::load_res fromimage::load(WZipReaderSingle &reader)
{
    QList<QByteArray> arr;
    QStringList name_list;
    uchar res;

    this->m_img.clear();

    name_list = this->get_name_img();

    if(this->load_metadata(reader) != load_res::ok)
        return load_res::err_meta_data;

    res = readListArray::read(name_list, *(reader.get_zip()), arr, false);
    if(res != OK){
        return fromimage::load_res::error;
    }

    return fromimage::load_multiple(arr);
}

fromimage::load_res fromimage::load_single(const QByteArray &arr,
                                           struct immagine_s &img)
{
    if(!img.immagini.loadFromData(arr, "PNG"))
        return load_res::error;

    if(img.immagini.isNull())
        return load_res::err_image_not_valid;

    return load_res::ok;
}

/*
 * in m_img sono già presenti tutte le immagini con i metadati
 * dobbiamo solo cambiare l'immagine e l'array
*/
fromimage::load_res fromimage::load_multiple(const QList<QByteArray> &arr)
{
    uint i, len;
    fromimage::load_res res;
    struct immagine_s img;

    len = arr.length();
    for(i=0; i<len; ++i){
        res = fromimage::load_single(arr.at(i), m_img.operator[](i));
        if(res != fromimage::load_res::ok)
            return res;
    }

    return fromimage::load_res::ok;
}

QStringList fromimage::get_name_img()
{
    return fromimage::get_name_img(*this->doc);
}

QStringList fromimage::get_name_img(const Document &doc)
{
    uint i;
    QStringList list;
    for(i=0; i<doc.count_img; ++i){
        list.append(fromimage::getName(i));
    }

    return list;
}

uchar fromimage::insert_image(QString &pos,
                              const PointSettable *point,
                              struct immagine_s &img)
{
    if(pos == ""){
        pos = QFileDialog::getOpenFileName(nullptr, "Open images", "", "Images (*.png *.xpm *.jpg *.jpeg);;All Files (*)");

        if(pos == "")
            return ERROR;
    }

    WImage immagine(pos);
    img.immagini = immagine;

    if(point){
        img.i = point->toPoint();
        img.f = point->toPoint() + QPoint(DELTA_POINT, DELTA_POINT);
    }
    else{
        img.i = QPoint(0, 0);
        img.f = QPoint(DELTA_POINT, DELTA_POINT);
    }

    return OK;
}

/*
 * add image from position
*/
int fromimage::addImage(QString &pos,
                         const PointSettable *point,
                         const QString &path_writernote)
{
    struct immagine_s img;
    WZipWriter writer;

    W_ASSERT(doc->count_img >= 0);

    if(writer.init(path_writernote.toUtf8().constData()))
        return -1;

    if(insert_image(pos, point, img) != OK)
        return -2;

    this->m_img.append(img);

    if(this->save(writer, pos) != fromimage::ok)
        return -3;

    this->doc->count_img ++;
    W_ASSERT(doc->count_img > 0);
    return 0;
}
