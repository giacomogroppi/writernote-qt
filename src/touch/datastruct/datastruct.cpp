#include "datastruct.h"
#include <QList>
#include <QDebug>
#include "../method/methoddefinition.h"

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

void datastruct::removeat(int i){
    x.removeAt(i);
    y.removeAt(i);
    color.removeAt(i);
    idtratto.removeAt(i);
    posizioneaudio.removeAt(i);
    pressure.removeAt(i);
    rotation.removeAt(i);
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

/* la funzione restituisce true se il pulto in questione è il primo o l'ultimo */
bool datastruct::needtochangeid(int i){
    int len;
    len = y.length();

    if(i == len || i == 0)
        return false;

    if(idtratto.at(i) == idtratto.at(i+1) && idtratto.at(i-1) == idtratto.at(i))
        return true;

    return false;

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
