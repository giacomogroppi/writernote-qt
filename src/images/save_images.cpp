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

    if(zip_source_write(file_zip, &len, sizeof(int)) == -1)
        return false;

    for(i=0; i<len; i++){
        /*buffer.open(QIODevice::WriteOnly);
        data->at(i).immagini.save(&buffer);

        temp = buffer.size();*/

        temp = data->at(i).immagini.sizeInBytes();

        if(zip_source_write(file_zip, &temp, sizeof(int)) == -1) return false;
        if(zip_source_write(file_zip, data->at(i).immagini.bits(), temp) == -1) return false;

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
