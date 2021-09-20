#include "xmlstruct.h"
#include "../utils/dialog_critic/dialog_critic.h"
#include <QDebug>

#define MESSAGE ""

/*
bool xmlstruct::loadindice(){
    indice->reset();

    zip_file *f;
    zip *filezip;
    int err = 0;
    int lunghezza, temp, i;
    char *nomefile;

    filezip = zip_open(this->path_->toUtf8().constData(),
                            ZIP_RDONLY,
                            &err);
    if (filezip == NULL){
        return false;
    }

    f = zip_fopen(filezip, "indice.xml", 0);

    if(!f){
        zip_close(filezip);
        return false;
    }

    zip_fread(f, &indice->versione, sizeof(int));

    if(indice->versione == VER_MULTIPLE_CURRENT_TITLE){

    }

    zip_fread(f, &lunghezza, sizeof(int));
    if(lunghezza){
        for(i=0; i<lunghezza; i++){
            zip_fread(f, &temp, sizeof(int));

            nomefile = new char[temp+1];

            zip_fread(f, nomefile, sizeof(char)*temp);

            nomefile[temp] = '\0';

            indice->titolo.append(nomefile);
            delete[] nomefile;
        }

    }

    zip_fclose(f);
    zip_close(filezip);

    return true;
}
*/
