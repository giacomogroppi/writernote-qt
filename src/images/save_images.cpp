#include "save_images.h"
#include "stdlib.h"
#include <QBuffer>
#include <QByteArray>

#include <QList>

#include "../datawrite/source_read_ext.h"

#define FIRST_SOURCE_READ(x, y, z) ARGUMENT(x,y,z)return ERROR;

int save_image(QList<struct immagine_S> *data, zip_source_t *file_zip)
{
    int len, i;
    size_t temp;
    len = data->length();

    SOURCE_WRITE_RETURN(file_zip, &len, sizeof(int));

    for(i=0; i<len; i++){
        temp = data->at(i).immagini.sizeInBytes();

        SOURCE_WRITE_RETURN(file_zip, &temp, sizeof(size_t));

        SOURCE_WRITE_RETURN(file_zip, data->at(i).immagini.bits(), temp);

        temp = data->at(i).i.x();
        SOURCE_WRITE_RETURN(file_zip, &temp, sizeof(int));

        temp = data->at(i).i.y();
        SOURCE_WRITE_RETURN(file_zip, &temp, sizeof(int));

        temp = data->at(i).f.x();
        SOURCE_WRITE_RETURN(file_zip, &temp, sizeof(int));

        temp = data->at(i).f.y();
        SOURCE_WRITE_RETURN(file_zip, &temp, sizeof(int));
    }

    return OK;
}

static int load_image_(struct immagine_S *temp_immagine,  zip_file_t *file_zip){
    void *data_read;

    QByteArray array;

    size_t temp;

    FIRST_SOURCE_READ(file_zip, &temp, sizeof(size_t));

    data_read = malloc(temp);

    if(!data_read)
        return ERROR;

    SOURCE_READ_GOTO(file_zip, data_read, temp);

    array.setRawData((const char *)data_read, temp);

    temp_immagine->immagini.loadFromData(array);

    SOURCE_READ_GOTO(file_zip, &temp, sizeof(int));
    temp_immagine->i.setX(temp);

    SOURCE_READ_GOTO(file_zip, &temp, sizeof(int));
    temp_immagine->i.setY(temp);

    SOURCE_READ_GOTO(file_zip, &temp, sizeof(int));
    temp_immagine->f.setX(temp);

    SOURCE_READ_GOTO(file_zip, &temp, sizeof(int));
    temp_immagine->f.setY(temp);


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

int load_image(QList<struct immagine_S> *data, zip_file_t *file_zip){
    int i, len;

    struct immagine_S *temp_immagine = new struct immagine_S;

    SOURCE_READ_GOTO(file_zip, &len, sizeof(int));

    for(i=0; i<len; i++){
        if(load_image_(temp_immagine, file_zip) != OK){
            goto free_;
        }

        data->append(*temp_immagine);
    }

    delete temp_immagine;
    return OK;

    free_:
    delete temp_immagine;
    return ERROR;
}
