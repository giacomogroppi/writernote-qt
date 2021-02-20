#include "save_images.h"
#include "stdlib.h"
#include <QBuffer>
#include <QByteArray>

#include <QList>

#include "../datawrite/source_read_ext.h"

int save_image(QList<struct immagine_S> *data, zip_source_t *file_zip)
{
    int len, i;
    size_t temp;
    len = data->length();

    QByteArray arr;
    QBuffer buffer(&arr);

    if(zip_source_write(file_zip, &len, sizeof(int)) == -1)
        return ERROR;

    for(i=0; i<len; i++){
        temp = data->at(i).immagini.sizeInBytes();

        SOURCE_WRITE(file_zip, &temp, sizeof(size_t));
        //if(zip_source_write(file_zip, &temp, sizeof(int)) == -1) return ERROR;

        SOURCE_WRITE(file_zip, data->at(i).immagini.bits(), temp);
        //if(zip_source_write(file_zip, data->at(i).immagini.bits(), temp) == -1) return ERROR;

        temp = data->at(i).i.x();
        SOURCE_WRITE(file_zip, &temp, sizeof(int));
        //if(zip_source_write(file_zip, &temp, sizeof(int)) == -1) return ERROR;

        temp = data->at(i).i.y();
        SOURCE_WRITE(file_zip, &temp, sizeof(int));
        //if(zip_source_write(file_zip, &temp, sizeof(int)) == -1) return ERROR;

        temp = data->at(i).f.x();
        SOURCE_WRITE(file_zip, &temp, sizeof(int));
        //if(zip_source_write(file_zip, &temp, sizeof(int)) == -1) return ERROR;

        temp = data->at(i).f.y();
        SOURCE_WRITE(file_zip, &temp, sizeof(int));
        //if(zip_source_write(file_zip, &temp, sizeof(int)) == -1) return ERROR;
    }

    return OK;
}

#define SOURCE_READ_EXT(x, y, z) check+=zip_fread(x, y, z) == -1

#define ARGUMENT(x, y, z) if(zip_fread(x, y,z)==-1)

#define SOURCE_READ(x, y, z) ARGUMENT(x,y,z)goto free_;
#define FIRST_SOURCE_READ(x, y, z) ARGUMENT(x,y,z)return ERROR;

static int load_image_(struct immagine_S *temp_immagine,  zip_file_t *file_zip){
    void *data_read;

    QByteArray array;

    size_t temp;

    FIRST_SOURCE_READ(file_zip, &temp, sizeof(size_t));
    //check += source_read_ext(file_zip, &temp, sizeof(int));

    data_read = malloc(temp);

    if(!data_read)
        return ERROR;

    SOURCE_READ(file_zip, data_read, temp);
    //check += source_read_ext(file_zip, data_read, temp);

    array.setRawData((const char *)data_read, temp);

    temp_immagine->immagini.loadFromData(array);

    SOURCE_READ(file_zip, &temp, sizeof(int));
    //check += source_read_ext(file_zip, &temp, sizeof(int));
    temp_immagine->i.setX(temp);

    SOURCE_READ(file_zip, &temp, sizeof(int));
    //check += source_read_ext(file_zip, &temp, sizeof(int));
    temp_immagine->i.setY(temp);

    SOURCE_READ(file_zip, &temp, sizeof(int));
    //check += source_read_ext(file_zip, &temp, sizeof(int));
    temp_immagine->f.setX(temp);

    SOURCE_READ(file_zip, &temp, sizeof(int));
    //check += source_read_ext(file_zip, &temp, sizeof(int));
    temp_immagine->f.setY(temp);


    free(data_read);
    return OK;

    free_:
    free(data_read);
    return ERROR;
}

int load_image(QList<struct immagine_S> *data, zip_file_t *file_zip){
    int i, len, check = 0;

    struct immagine_S *temp_immagine = new struct immagine_S;

    check += source_read_ext(file_zip, &len, sizeof(int));

    for(i=0; i<len; i++){
        if(load_image_(temp_immagine, file_zip) != OK){
            delete temp_immagine;
            return -1;
        }

        data->append(*temp_immagine);
    }

    delete temp_immagine;

    return check;
}
