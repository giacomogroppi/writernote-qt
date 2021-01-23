#include "save_images.h"
#include "../datawrite/source_read_ext.h"

static bool load_image_(struct immagine_S *temp_immagine,  zip_file_t *file_zip){
    void *data_read;

    QByteArray array;

    int temp, check = 0;

    check += source_read_ext(file_zip, &temp, sizeof(int));

    data_read = malloc(temp);

    if(!data_read)
        return false;


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
