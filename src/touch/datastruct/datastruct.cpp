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
