#include "savefile.h"
#include "zip.h"

/*funzione che gestisce il salvataggio dell'indice*/
bool savefile::compressfile(const char *namefile, const char *text){
/*
    namefile: nome file da salvare ("indice.xml", "file2.xml")
    text: testo da scrivere
*/
    /*int errorp = 0;

    zip_t *filezip = zip_open(this->parent->self->path.c_str(), ZIP_CREATE , &errorp);

    if (filezip == nullptr) {
            zip_error_t ziperror;
            zip_error_init_with_code(&ziperror, errorp);
            throw std::runtime_error("Failed to open output file " + this->parent->self->path + ": " + zip_error_strerror(&ziperror));
    }

    zip_source_t *temp;
    int lunghezzastringa = strlen(text);

    temp = zip_source_buffer(filezip, text, sizeof(char)*lunghezzastringa, 0);


    if ( temp == NULL || zip_file_add(filezip, namefile, temp, 0) < 0) {
        int indice = zip_name_locate(filezip, namefile, 0);
        if(zip_file_replace(filezip, indice, temp, 0) == -1){
            zip_source_free(temp);
            return false;
        }
    }

    zip_close(filezip);*/

    return this->compressfile(text, sizeof (char) * strlen(text), namefile);
}

/* la funzione comprime un file */
bool savefile::compressfile(const void *dato, int size, const char *namefile){
    int errorp = 0;

    zip_t *filezip = zip_open(this->parent->self->path.toUtf8().constData(), ZIP_CREATE , &errorp);

    if (filezip == nullptr) {
        return false;
    }

    zip_source_t *temp;

    temp = zip_source_buffer(filezip, dato, size, 0);

    /* ZIP_FL_ENC_UTF_8 */

    if ( temp == NULL || zip_file_add(filezip, namefile, temp, 0) < 0) {
        int indice = zip_name_locate(filezip, namefile, 0);
        if(zip_file_replace(filezip, indice, temp, 0) == -1){
            zip_source_free(temp);
            //qDebug() << "\nsavefile::compressfile -> Abbiamo avuto un problema nel file replace " << zip_strerror(filezip) << endl;
            return false;
        }
    }

    zip_close(filezip);

    return true;
}
