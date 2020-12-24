#include "datastruct.h"
#include <QList>
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
    len = this->x.length();
    for(i=0; i<len; i++)
        qDebug() << "\nx:" << this->x.at(i) << "\ny: " << this->y.at(i);

}

int datastruct::firstx(){
    int i, min, len;
    len = x.length();
    min = x.first();
    for(i=0; i < len; i++)
        if(min > x.at(i))
            min = x.at(i);
    return min;

}

int datastruct::biggerx(){
    int i, len = x.length();
    short int max;

    max = x.first();
    for(i=0; i < len; i++)
        if(max < x.at(i))
            max = x.at(i);

    return max;

}

/* la funzione restituisce il massimo valore per le y */
int datastruct::last(){
    if(!y.length())
        return 0;

    int i, len = this->y.length(), max = this->y.at(0);
    for(i = 0; i < len; i++){
        if(max < this->y.at(i) && idtratto.at(i) != -1)
            max = this->y.at(i);
    }

    return max;
}

int datastruct::firsty(){
    int i, len = this->y.length(), min = this->y.at(0);
    for(i = 0; i < len; i++){
        if(min > this->y.at(i))
            min = this->y.at(i);
    }
    return min;
}
