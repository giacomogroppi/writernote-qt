#include "redoundo.h"

#include "../currenttitle/checksimilecopybook.h"

static void copycolor(QList<struct colore_s> *, QList<struct colore_s> *);
static void copycoordinate(QList<double> *, QList<double> *);
static void copypressure(QList<float> *, QList<float> *);
static void copyidtratto(QList<int> *, QList<int> *);

void redoundo::copy_b(currenttitle_class *dest, currenttitle_class *src){
    int temp = COLORE;
    bool ok;

    do{
        temp = checksimilecopybook(src, dest);
        ok = false;

        switch (temp) {
            case COLORE:
                copycolor(&src->datatouch->color, &dest->datatouch->color);
                ok = true;
                break;
            case XCHECK:
                copycoordinate(&src->datatouch->x, &dest->datatouch->x);
                ok = true;
                break;
            case YCHECK:
                copycoordinate(&src->datatouch->y, &dest->datatouch->y);
                ok = true;
                break;
            case PRESSURE:
                copypressure(&src->datatouch->pressure, &dest->datatouch->pressure);
                ok = true;
                break;
            case IDTRATTO:
                copyidtratto(&src->datatouch->idtratto, &dest->datatouch->idtratto);
                ok = true;
                break;
        }

        if(!ok)
            break;

    }while(temp != OK);
}

void redoundo::copy(){
    this->scala(1);

    copy_b(this->m_lista.at(indice), m_current);

    this->indice ++;
}

/* TODO -> understand what has changed */
void copycolor(QList<struct colore_s> *src, QList<struct colore_s> *dest){
    int i, len;
    len = src->length();

    dest->clear();

    for(i=0; i<len; i++){
        dest->append(src->at(i));
    }
}

void copycoordinate(QList<double> *src, QList<double> *dest){
    int i, len;
    len = src->length();

    dest->clear();

    for(i=0; i<len; i++){
        dest->append(src->at(i));
    }
}

void copypressure(QList<float> *src, QList<float> *dest){
    int i, len;
    len = src->length();

    dest->clear();
    for(i=0; i<len; i++){
        dest->append(src->at(i));
    }
}

void copyidtratto(QList<int> *src, QList<int> *dest){
    int i, len;
    len = src->length();

    dest->clear();

    for(i=0; i<len; i++){
        dest->append(src->at(i));
    }
}
