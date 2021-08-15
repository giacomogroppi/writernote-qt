#include "fromimage.h"
#include "stdlib.h"
#include <QBuffer>
#include <QByteArray>
#include <QList>
#include <QFileDialog>

#include "../dataread/readlistarray.h"
#include "../datawrite/source_read_ext.h"
#include "../currenttitle/document.h"
#include "../datawrite/savefile.h"

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
    if(savefile::saveArrayIntoFile(path,
                                   doc->nome_copybook,
                                   path_writernote_file,
                                   file,
                                   fromimage::getNameNoCopy(doc->count_img)) != OK)
        return load_res::error;
    return load_res::ok;
}

fromimage::load_res fromimage::load(zip_t *file, const bool clear){
    QList<QByteArray> arr;
    QStringList name_list;
    uchar res;

    if(clear){
        this->m_img.clear();
    }

    name_list = this->get_name_img();

    res = readListArray::read(name_list, file, arr, clear);

    if(res != OK){
        return fromimage::load_res::error;
    }

    return fromimage::load_multiple(arr, this->m_img);
}

fromimage::load_res fromimage::load_single(const QByteArray &arr, immagine_S &img)
{
    const size_t size_meta = sizeof(double)*4;
    QByteArray arr_mid;
    double d[4];
    uint i;
    char val;

    for(i=0; i<size_meta; ++i){
        val = arr.at(i);
        memcpy(&d[i],  &val, sizeof(char));
    }
    img.i = QPointF(d[0], d[1]);
    img.f = QPointF(d[2], d[3]);

    arr_mid = arr.mid(size_meta);

    img.immagini.loadFromData(arr_mid);

    return load_res::ok;
}

fromimage::load_res fromimage::load_multiple(const QList<QByteArray> &arr, QList<immagine_S> &img_l)
{
    uint i, len;
    fromimage::load_res res;
    struct immagine_S img;

    len = arr.length();
    for(i=0; i<len; ++i){
        res = fromimage::load_single(arr.at(i), img);
        if(res != fromimage::load_res::ok)
            return res;
        img_l.append(img);
    }

    return fromimage::load_res::ok;
}

int save_image(QList<struct immagine_S> *data,
               zip_source_t *file_zip)
{
    uint len, i, temp_i;
    size_t temp_s;
    len = data->length();

    SOURCE_WRITE_RETURN(file_zip, &len, sizeof(int));

    for(i=0; i<len; i++){
        temp_s = data->at(i).immagini.sizeInBytes();

        SOURCE_WRITE_RETURN(file_zip, &temp_s, sizeof(size_t));

        SOURCE_WRITE_RETURN(file_zip, data->at(i).immagini.bits(), temp_s);

        temp_i = data->at(i).i.x();
        SOURCE_WRITE_RETURN(file_zip, &temp_i, sizeof(int));

        temp_i = data->at(i).i.y();
        SOURCE_WRITE_RETURN(file_zip, &temp_i, sizeof(int));

        temp_i = data->at(i).f.x();
        SOURCE_WRITE_RETURN(file_zip, &temp_i, sizeof(int));

        temp_i = data->at(i).f.y();
        SOURCE_WRITE_RETURN(file_zip, &temp_i, sizeof(int));
    }

    return OK;
}

static int load_image_(struct immagine_S &temp_immagine,
                       zip_file_t *file_zip){
    void *data_read;

    QByteArray array;

    size_t temp_s;
    int temp_i;

    FIRST_SOURCE_READ(file_zip, &temp_s, sizeof(size_t));

    data_read = malloc(temp_s);

    if(!data_read)
        return ERROR;

    SOURCE_READ_GOTO(file_zip, data_read, temp_s);

    array.setRawData((const char *)data_read, temp_s);

    temp_immagine.immagini.loadFromData(array);

    SOURCE_READ_GOTO(file_zip, &temp_i, sizeof(int));
    temp_immagine.i.setX(temp_i);

    SOURCE_READ_GOTO(file_zip, &temp_i, sizeof(int));
    temp_immagine.i.setY(temp_i);

    SOURCE_READ_GOTO(file_zip, &temp_i, sizeof(int));
    temp_immagine.f.setX(temp_i);

    SOURCE_READ_GOTO(file_zip, &temp_i, sizeof(int));
    temp_immagine.f.setY(temp_i);


    free(data_read);
    return OK;

    /*
     * we cannot return ERROR directly
     * otherwise the memory allocated
     * with the malloc is lost
    */
    free_:
    free(data_read);
    return ERROR;
}

int load_image(QList<struct immagine_S> *data,
               zip_file_t *file_zip){
    int i, len;
    struct immagine_S temp_immagine;

    SOURCE_READ_GOTO(file_zip, &len, sizeof(int));

    for(i=0; i<len; i++){
        if(load_image_(temp_immagine, file_zip) != OK){
            goto free_;
        }

        data->append(temp_immagine);
    }

    return OK;

    free_:
    return ERROR;
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

    struct immagine_S *immagine_temp = new struct immagine_S;
    QImage immagine(pos);
    immagine_temp->immagini = immagine;

    if(point){
        immagine_temp->i = point->point.toPoint();
        immagine_temp->f = point->point.toPoint() + QPoint(DELTA_POINT, DELTA_POINT);
    }
    else{
        immagine_temp->i = QPoint(0, 0);
        immagine_temp->f = QPoint(DELTA_POINT, DELTA_POINT);
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

    this->doc->count_img ++;

    this->m_img.append(img);

    this->save(nullptr, pos, path_writernote);
}
