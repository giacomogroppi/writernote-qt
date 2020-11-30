#ifndef INDICE_CLASS_CPP
#define INDICE_CLASS_CPP
#include <iostream>
#include "indice_class.h"
#include <QDebug>
/*funzione per gestire l'indice*/

/*void indice_class::newfile(char video[MAXFILESTR], char audio[MAXFILESTR],
                           char titolo[MAXFILESTR], char file_testo[MAXFILESTR]){
	 

}*/
void indice_class::reset(){
    this->versione=0;
    this->video.clear();
    this->audio.clear();
    this->titolo.clear();
    this->compressione.clear();
}


void indice_class::stampa(){
    int i, lunghezza = this->titolo.length();
    std::cout << std::endl << std::endl << "Indice: ->";
    for(i=0; i<lunghezza; i++){
        qDebug() << "titolo" << this->titolo[i];
        qDebug() << "video" << this->video[i];
        qDebug() << "audio" << this->audio[i];
        qDebug() << "compressione" << this->compressione[i];
    }
}




#endif //INDICE_CLASS_CPP
