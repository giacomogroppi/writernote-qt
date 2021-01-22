#include "save_images.h"
#include "stdlib.h"
#include <QBuffer>
#include <QByteArray>

#include <QList>

#include "../datawrite/source_read_ext.h"

int save_image(QList<struct immagine_S> *data, zip_source_t *file_zip)
{
    int len, i, temp;
    len = data->length();

    QByteArray arr;
    QBuffer buffer(&arr);

    temp = data->length();
    if(zip_source_write(file_zip, &temp, sizeof(int)) < 0)
        return false;

    for(i=0; i<len; i++){
        buffer.open(QIODevice::WriteOnly);
        data->at(i).immagini.save(&buffer);

        temp = buffer.size();
        if(zip_source_write(file_zip, &temp, sizeof(int)) == -1) return false;
        if(zip_source_write(file_zip, &buffer, buffer.size()) == -1) return false;

        temp = data->at(i).i.x();
        if(zip_source_write(file_zip, &temp, sizeof(int)) == -1) return false;

        temp = data->at(i).i.y();
        if(zip_source_write(file_zip, &temp, sizeof(int)) == -1) return false;

        temp = data->at(i).f.x();
        if(zip_source_write(file_zip, &temp, sizeof(int)) == -1) return false;

        temp = data->at(i).f.y();
        if(zip_source_write(file_zip, &temp, sizeof(int)) == -1) return false;
    }

    return true;
}

static bool load_image_(struct immagine_S *temp_immagine,  zip_file_t *file_zip){
    void *data_read;

    QByteArray array;

    int temp, check = 0;

    //check += zip_fread(file_zip, &temp, sizeof(int));

    check += source_read_ext(file_zip, &temp, sizeof(int));

    data_read = malloc(temp);

    check += source_read_ext(file_zip, data_read, temp);

    array.setRawData((const char *)data_read, temp);

    temp_immagine->immagini.loadFromData(array);

    check += source_read_ext(file_zip, &temp, sizeof(int));
    temp_immagine->i.setX(temp);

    check += source_read_ext(file_zip, &temp, sizeof(int));
    temp_immagine->i.setY(temp);

    check += source_read_ext(file_zip, &temp, sizeof(int));
    temp_immagine->f.setX(temp);

    check += source_read_ext(file_zip, &temp, sizeof(int));
    temp_immagine->f.setY(temp);

    return check < 0;
}

int load_image(QList<struct immagine_S> *data, zip_file_t *file_zip){
    int i, len, check = 0;

    struct immagine_S *temp_immagine = new struct immagine_S;

    check += source_read_ext(file_zip, &len, sizeof(int));

    for(i=0; i<len; i++){
        if(load_image_(temp_immagine, file_zip)){
            delete temp_immagine;
            return -1;
        }

        data->append(*temp_immagine);
    }

    delete temp_immagine;

    return check;
}
