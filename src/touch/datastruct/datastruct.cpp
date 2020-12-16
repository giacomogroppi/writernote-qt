#include "datastruct.h"

#include <QDebug>

datastruct::datastruct()
{

}

void datastruct::reset(){
    this->color.clear();
    this->x.clear();
    this->y.clear();
    this->posizioneaudio.clear();
}

void datastruct::stampa(){

    int i, len;
    len = this->color.length();
    for(i=0; i<len; i++)
        qDebug() << this->color.at(i);

    len = this->x.length();
    for(i=0; i<len; i++)
        qDebug() << "\nx:" << this->x.at(i) << "\ny: " << this->y.at(i);

}

/* la funzione restituisce il massimo valore per le y */
int datastruct::last(){
    int i, len = this->y.length(), max = this->y.at(0);
    for(i = 0; i < len; i++){
        if(max > this->y.at(i))
            max = this->y.at(i);
    }
    return max;
}

int datastruct::first(){
    int i, len = this->y.length(), max = this->y.at(0);
    for(i = 0; i < len; i++){
        if(max > this->y.at(i))
            max = this->y.at(i);
    }
    return max;
}
