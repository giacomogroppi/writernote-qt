#include "fromimage.h"
#include "stdlib.h"
#include <QBuffer>
#include <QByteArray>
#include <QList>
#include <QFileDialog>
#include "zip.h"
#include "../dataread/readlistarray.h"
#include "../datawrite/source_read_ext.h"
#include "../currenttitle/document.h"
#include "../datawrite/savefile.h"
#include "../dataread/xmlstruct.h"
#include "../dataread/load_from_file.h"

#define FIRST_SOURCE_READ(x, y, z) ARGUMENT(x,y,z)return ERROR;

fromimage::load_res fromimage::save(zip_t *file,
                                    const QStringList &path,
                                    const QString &path_writernote_file) const{
    fromimage::load_res res;
    uint i, len;

    doc->count_img = path.length();
    len = doc->count_img;

    for(i=0; i<len; ++i){
        res = this->save(file,
                         path.at(i),
                         path_writernote_file);
        if(res != fromimage::load_res::ok)
            return res;
    }
    return fromimage::load_res::ok;
}

fromimage::load_res fromimage::save(zip_t *file,
                                    const QString &path,
                                    const QString &path_writernote_file) const{

    QByteArray img_in_byte;
    QImage img;

    if(this->get_img_bytearray(img_in_byte, path) != load_res::ok){
        return load_res::error;
    }

    img.loadFromData(img_in_byte, "PNG");
    if(img.isNull())
        return load_res::err_image_not_valid;

    if(savefile::saveArrayIntoFile(img_in_byte,
                                   doc->nome_copybook,
                                   path_writernote_file,
                                   file,
                                   fromimage::getNameNoCopy(doc->count_img)) != OK)
        return load_res::error;

    return load_res::ok;
}

fromimage::load_res fromimage::save_metadata(zip_source_t *file)
{
    uint i;
    double val[4];

    for(i=0; i<doc->count_img; ++i){
        const struct immagine_S &img = m_img.at(i);

        val[0] = img.i.x();
        val[1] = img.i.y();
        val[2] = img.f.x();
        val[3] = img.f.y();

        if(zip_source_write(file, val, sizeof(double)*4) == -1)
            return load_res::err_meta_data;
    }

    return load_res::ok;
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

fromimage::load_res fromimage::load_metadata(zip_file_t *file)
{
    uint i;
    double val[4];
    struct immagine_S img;

    for(i=0; i<this->doc->count_img; ++i){
        if(zip_fread(file, val, sizeof (double)*4) == -1)
            return load_res::error;
        img.i = QPointF(val[0], val[1]);
        img.f = QPointF(val[2], val[3]);

        m_img.append(img);
    }

    return load_res::ok;
}

fromimage::load_res fromimage::load(zip_t *filezip,
                                    zip_file_t *file){
    QList<QByteArray> arr;
    QStringList name_list;
    uchar res;

    this->m_img.clear();

    name_list = this->get_name_img();

    if(this->load_metadata(file) != load_res::ok)
        return load_res::err_meta_data;

    res = readListArray::read(name_list, filezip, arr, false);
    if(res != OK){
        return fromimage::load_res::error;
    }

    return fromimage::load_multiple(arr);
}

fromimage::load_res fromimage::load_single(const QByteArray &arr,
                                           struct immagine_S &img)
{
    if(!img.immagini.loadFromData(arr, "PNG"))
        return load_res::error;

    if(img.immagini.isNull())
        return load_res::err_image_not_valid;

    return load_res::ok;
}

fromimage::load_res fromimage::load_multiple(const QList<QByteArray> &arr)
{
    uint i, len;
    fromimage::load_res res;
    struct immagine_S img;

    len = arr.length();
    for(i=0; i<len; ++i){
        res = fromimage::load_single(arr.at(i), img);
        if(res != fromimage::load_res::ok)
            return res;
        m_img.append(img);
    }

    return fromimage::load_res::ok;
}

QStringList fromimage::get_name_img()
{
    uint i;
    QStringList list;
    for(i=0; i<doc->count_img; ++i){
        list.append(fromimage::getName(doc->nome_copybook, i));
    }

    return list;
}

uchar fromimage::insert_image(QString &pos,
                              const PointSettable *point,
                              struct immagine_S &img)
{
    if(pos == ""){
        pos = QFileDialog::getOpenFileName(nullptr,
                                                     "Open images",
                                                     "JPEG (*.jpg, *.jpeg);;PNG (*.png);;All Files (*)");

        if(pos == "")
            return ERROR;
    }

    QImage immagine(pos);
    img.immagini = immagine;

    if(point){
        img.i = point->point.toPoint();
        img.f = point->point.toPoint() + QPoint(DELTA_POINT, DELTA_POINT);
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
void fromimage::addImage(QString &pos,
                         const PointSettable *point,
                         const QString &path_writernote)
{
    struct immagine_S img;

    if(insert_image(pos, point, img) != OK)
        return;

    this->m_img.append(img);

    this->save(nullptr, pos, path_writernote);
    this->doc->count_img ++;
}
