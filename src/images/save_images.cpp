#include "save_images.h"
#include "stdlib.h"
#include <QBuffer>
#include <QByteArray>

int save_image(datastruct *data, zip_source_t *file_zip)
{
    int len, i, check, temp;
    len = data->immagini.length();

    QByteArray arr;
    QBuffer buffer(&arr);

    temp = data->immagini.length();
    check += zip_source_write(file_zip, &temp, sizeof(int));

    for(i=0, check = 0; i<len && check == 0; i++){
        buffer.open(QIODevice::WriteOnly);
        data->immagini.at(i).immagini.save(&buffer);

        temp = buffer.size();
        check += zip_source_write(file_zip, &temp, sizeof(int));
        check += zip_source_write(file_zip, &buffer, buffer.size());

        temp = data->immagini.at(i).i.x();
        check += zip_source_write(file_zip, &temp, sizeof(int));

        temp = data->immagini.at(i).i.y();
        check += zip_source_write(file_zip, &temp, sizeof(int));

        temp = data->immagini.at(i).f.x();
        check += zip_source_write(file_zip, &temp, sizeof(int));

        temp = data->immagini.at(i).f.y();
        check += zip_source_write(file_zip, &temp, sizeof(int));
    }

    return check;
}

int load_image(datastruct *data, zip_file_t *file_zip){
    int i, len, check = 0, temp;

    void *data_read = malloc(1);

    QImage immagine;
    QByteArray array;
    struct immagine_S immagine_append;


    if(!data_read)
        return -1;

    check += zip_fread(file_zip, &len, sizeof(int));

    for(i=0; i<len; i++){
        check += zip_fread(file_zip, &temp, sizeof(int));
        data_read = realloc(data_read, sizeof(temp));

        check += zip_fread(file_zip, data_read, temp);

        array.setRawData((const char *)data_read, temp);

        immagine_append.immagini.loadFromData(array);

        check += zip_fread(file_zip, &temp, sizeof(int));
        immagine_append.i.setX(temp);

        check += zip_fread(file_zip, &temp, sizeof(int));
        immagine_append.i.setY(temp);

        check += zip_fread(file_zip, &temp, sizeof(int));
        immagine_append.f.setX(temp);

        check += zip_fread(file_zip, &temp, sizeof(int));
        immagine_append.f.setY(temp);
    }

    return check;
}
