#include "fromimage.h"
#include "stdlib.h"
#include <QBuffer>
#include <QByteArray>

#include <QList>
#include <QFileDialog>

#include "../datawrite/source_read_ext.h"
#include "../currenttitle/document.h"

#define FIRST_SOURCE_READ(x, y, z) ARGUMENT(x,y,z)return ERROR;

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

immagine_S * fromimage::insert_image(const char *__pos, PointSettable *point)
{
    QString posizionefoto;

    if(!__pos){
        posizionefoto = QFileDialog::getOpenFileName(nullptr,
                                                     "Open images",
                                                     "JPEG (*.jpg, *.jpeg);;PNG (*.png);;All Files (*)");

        if(posizionefoto == "")
            return NULL;
    }

    struct immagine_S *immagine_temp = new struct immagine_S;
    QImage immagine(posizionefoto);
    immagine_temp->immagini = immagine;

    if(point){
        immagine_temp->i = point->point.toPoint();
        immagine_temp->f = point->point.toPoint() + QPoint(delta_point, delta_point);
    }
    else{
        immagine_temp->i = QPoint(0, 0);
        immagine_temp->f = QPoint(delta_point, delta_point);
    }
    return immagine_temp;
}

/*
 * add image from position
*/
void fromimage::addImage(Document *m_currenttitle, const char *__pos, PointSettable *point)
{
    struct immagine_S *immagine = insert_image(__pos, point);
    if(!immagine)
        return;


    this->m_img.append(*immagine);

    delete immagine;
}
