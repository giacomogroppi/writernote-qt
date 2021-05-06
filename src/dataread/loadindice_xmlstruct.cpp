#include "xmlstruct.h"
#include "../utils/dialog_critic/dialog_critic.h"

bool xmlstruct::loadindice(){
    indice->reset();

    int err = 0;
    int lunghezza, temp, i;

    zip *filezip = zip_open(this->path_->toUtf8().constData(),
                            ZIP_RDONLY,
                            &err);
    if (filezip == NULL){
        dialog_critic("First");
        return false;
    }

    zip_file *f = zip_fopen(filezip, "indice.xml", 0);

    if(!f){
        zip_close(filezip);
        return false;
    }

    zip_fread(f, &indice->versione, sizeof(int));

    zip_fread(f, &lunghezza, sizeof(int));
    if(lunghezza){
        char *nomefile;

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
