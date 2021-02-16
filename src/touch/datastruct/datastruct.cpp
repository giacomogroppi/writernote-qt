#include "datastruct.h"
#include <QList>
#include "../method/methoddefinition.h"

datastruct::datastruct()
{

}

void datastruct::reset(){
    this->color.clear();
    this->x.clear();
    this->y.clear();
    this->posizioneaudio.clear();
    this->idtratto.clear();
    this->zoom = 1.00;
    this->posizionefoglio.clear();
}

double datastruct::biggery(){
    int i, len;
    double y_ = y.first();
    len = y.length();
    for(i=0;i<len;i++)
        if(y.at(i) > y_)
            y_ = y.at(i);
    return y_;
}


auto datastruct::biggerynoid() -> double{
    int i, len;
    double y_ = y.first();
    len = y.length();
    for(i=0;i<len;i++)
        if(y.at(i) > y_ && idtratto.at(i) != -1)
            y_ = y.at(i);
    return y_;
}

int datastruct::firstx(){
    int i, min, len;
    len = x.length();
    min = x.first();
    for(i=0; i < len; i++)
        if(min > x.at(i) && idtratto.at(i) != -1)
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


double datastruct::biggerx(){
    int i, len;

    len = x.length();

    double max;
    if(!len)
        return 0;
    max = x.first();
    for(i=0; i < len; i++)
        if(max < x.at(i))
            max = x.at(i);

    return max;

}

bool datastruct::ispositivex(){
    if(!x.length())
        return true;

    int i, len;
    len = x.length();

    for(i=0; i<len; i++)
        if(x.at(i) < 0)
            return false;
    return true;
}

bool datastruct::ispositivey(){
    if(!x.length())
        return true;

    int i, len;
    len = x.length();

    for(i=0; i<len; i++)
        if(y.at(i) < 0)
            return false;
    return true;

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
