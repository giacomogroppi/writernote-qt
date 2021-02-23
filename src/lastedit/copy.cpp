#include "redoundo.h"

#include "../currenttitle/checksimilecopybook.h"

/*
static void copycolor(QList<struct colore_s> *, QList<struct colore_s> *);
static void copycoordinate(QList<double> *, QList<double> *);
static void copypressure(QList<float> *, QList<float> *);
static void copyidtratto(QList<int> *, QList<int> *);*/


static void copycolor(QList<struct point_s> *, QList<struct point_s> *);
static void copycoordinate(QList<struct point_s> *, QList<struct point_s> *);
static void copypressure(QList<struct point_s> *, QList<struct point_s> *);
static void copyidtratto(QList<struct point_s> *, QList<struct point_s> *);

void redoundo::copy_b(currenttitle_class *dest, currenttitle_class *src){
    int temp = COLORE;
    bool ok;

    do{
        temp = checksimilecopybook(src, dest);
        ok = false;

        switch (temp) {
            case COLORE:
                copycolor(&src->datatouch->m_point, &dest->datatouch->m_point);
                /*copycolor(&src->datatouch->color, &dest->datatouch->color);*/
                ok = true;
                break;
            case XCHECK:
                copycoordinate(&src->datatouch->m_point, &dest->datatouch->m_point);
                /*copycoordinate(&src->datatouch->x, &dest->datatouch->x);*/
                ok = true;
                break;
            case YCHECK:
                copycoordinate(&src->datatouch->m_point, &dest->datatouch->m_point);
                /*copycoordinate(&src->datatouch->y, &dest->datatouch->y);*/
                ok = true;
                break;
            case PRESSURE:
                copypressure(&src->datatouch->m_point, &dest->datatouch->m_point);
                /*copypressure(&src->datatouch->pressure, &dest->datatouch->pressure);*/
                ok = true;
                break;
            case IDTRATTO:
                copyidtratto(&src->datatouch->m_point, &dest->datatouch->m_point);
                /*copyidtratto(&src->datatouch->idtratto, &dest->datatouch->idtratto);*/
                ok = true;
                break;
        }

        if(!ok)
            break;

    }while(temp != OK_CHECK);
}

void redoundo::copy(){
    this->scala(1);

    copy_b(this->m_lista.at(indice), m_current);

    this->indice ++;
}

static void copycolor(QList<struct point_s> *src, QList<struct point_s> *dest){
    int i, len = src->length();


    for(i=0; i<len; i++){
    }

}
static void copycoordinate(QList<struct point_s> *, QList<struct point_s> *);
static void copypressure(QList<struct point_s> *, QList<struct point_s> *);
static void copyidtratto(QList<struct point_s> *, QList<struct point_s> *);


/*
 * the function is comment because it didn't work with the new datastruct
*/

/* TODO -> understand what has changed */
/*static void copycolor(QList<struct colore_s> *src, QList<struct colore_s> *dest){
    int i, len;
    len = src->length();

    dest->clear();

    for(i=0; i<len; i++){
        dest->append(src->at(i));
    }
}

static void copycoordinate(QList<double> *src, QList<double> *dest){
    int i, len;
    len = src->length();

    dest->clear();

    for(i=0; i<len; i++){
        dest->append(src->at(i));
    }
}

static void copypressure(QList<float> *src, QList<float> *dest){
    int i, len;
    len = src->length();

    dest->clear();
    for(i=0; i<len; i++){
        dest->append(src->at(i));
    }
}

static void copyidtratto(QList<int> *src, QList<int> *dest){
    int i, len;
    len = src->length();

    dest->clear();

    for(i=0; i<len; i++){
        dest->append(src->at(i));
    }
}
*/
