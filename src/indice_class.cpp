#include <iostream>
#include "indice_class.h"
#include <QDebug>

void indice_class::reset(){
    this->versione=0;
    this->titolo.clear();
}


void indice_class::stampa(){
    int i, lunghezza = this->titolo.length();
    qDebug() << "Indice: ->";
    for(i=0; i<lunghezza; i++){
        qDebug() << "titolo" << this->titolo[i];
    }
}

