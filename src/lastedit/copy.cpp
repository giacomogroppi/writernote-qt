#include "redoundo.h"

#include "../currenttitle/checksimilecopybook.h"

static inline void copycolor(QList<struct point_s> *, QList<struct point_s> *);

static inline void copycoordinatey(QList<struct point_s> *, QList<struct point_s> *);
static inline void copycoordinatex(QList<struct point_s> *, QList<struct point_s> *);

static inline void copypressure(QList<struct point_s> *, QList<struct point_s> *);
static inline void copyidtratto(QList<struct point_s> *, QList<struct point_s> *);

static inline void copyall(QList<struct point_s>  *, QList<struct point_s> *);

void redoundo::copy_b(currenttitle_class *dest, currenttitle_class *src){
    int temp = COLORE;
    bool ok;

    do{
        temp = checksimilecopybook(src, dest, true);
        ok = false;

        switch (temp) {
            case LEN:
                copyall(&src->datatouch->m_point, &dest->datatouch->m_point);
                break;

            case COLORE:
                copycolor(&src->datatouch->m_point, &dest->datatouch->m_point);
                /*copycolor(&src->datatouch->color, &dest->datatouch->color);*/
                ok = true;
                break;

            case XCHECK:
                copycoordinatex(&src->datatouch->m_point, &dest->datatouch->m_point);
                /*copycoordinate(&src->datatouch->x, &dest->datatouch->x);*/
                ok = true;
                break;

            case YCHECK:
                copycoordinatey(&src->datatouch->m_point, &dest->datatouch->m_point);
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

            case LEN_POSIZIONEFOGLIO:
            {
                int i, len = src->datatouch->posizionefoglio.length();
                dest->datatouch->posizionefoglio.clear();
                for(i=0; i<len; i++){
                    dest->datatouch->posizionefoglio << src->datatouch->posizionefoglio.at(i);
                }
                break;
            }
            case POSIZIONE_FOGLIO:
            {
                int i, len = src->datatouch->posizionefoglio.length();
                for(i=0; i<len; i++){
                    dest->datatouch->posizionefoglio.operator[](i) = src->datatouch->posizionefoglio.at(i);
                }
            }
        }

        if(!ok)
            break;

    }while(temp != OK_CHECK);
}

void redoundo::copy(){
    this->scala(1);

    return;
    /* not working */
    copy_b(this->m_lista.operator[](indice), m_current);

    this->indice ++;
}

static inline void copycolor(QList<struct point_s> *src, QList<struct point_s> *dest){
    int i, len = src->length();


    for(i=0; i<len; i++){
        memcpy(&dest->operator[](i).m_color, &src->operator[](i).m_color, sizeof(colore_s));
    }
}

static inline void copycoordinatey(QList<struct point_s> *src, QList<struct point_s> *dest){
    int i, len;
    for(i=0, len = src->length(); i<len; i++){
        dest->operator[](i).m_y = src->operator[](i).m_y;
    }
}

static inline void copycoordinatex(QList<struct point_s> *src, QList<struct point_s> *dest){
    int i, len;
    for(i=0, len = src->length(); i<len; i++){
        dest->operator[](i).m_x = src->operator[](i).m_x;
    }
}

static inline void copypressure(QList<struct point_s> *src, QList<struct point_s> *dest){
    int i, len = src->length();
    for(i=0; i<len; i++){
        dest->operator[](i).m_pressure = src->operator[](i).m_pressure;
    }
}

static inline void copyidtratto(QList<struct point_s> *src, QList<struct point_s> *dest){
    int i, len;
    for(i=0, len = src->length(); i<len; i++){
        dest->operator[](i).idtratto = src->operator[](i).idtratto;
    }
}

static inline void copyall(QList<struct point_s>  *src, QList<struct point_s> *dest){
    dest->clear();
    *dest = *src;
}



