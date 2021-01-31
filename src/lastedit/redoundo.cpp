#include "redoundo.h"

#include "../currenttitle/checksimilecopybook.h"

static void copy_c(currenttitle_class *, currenttitle_class *);
static void copycolor(QList<struct colore_s> *, QList<struct colore_s> *);
static void copycoordinate(QList<double> *, QList<double> *);
static void copypressure(QList<float> *, QList<float> *);
static void copyidtratto(QList<int> *, QList<int> *);

redoundo::redoundo(currenttitle_class *data)
{
    this->current = data;
}

void redoundo::redo(){
    if(this->check)
        return;
    copy_c(&this->next, this->current);
}


void redoundo::copy(){
    copy_c(this->current, &this->last);
}

/* reset the currenttitle */
void redoundo::undo(){
    copy_c(&this->last, this->current);

    this->check = true;
}


static void copy_c(currenttitle_class *src, currenttitle_class *dest){
    int temp = COLORE;
    while(temp != OK){
        temp = checksimilecopybook(src, dest);
        if(temp == OK)
            return;

        switch (temp) {
            case COLORE:
                copycolor(&src->datatouch->color, &dest->datatouch->color);
                break;
            case XCHECK:
                copycoordinate(&src->datatouch->x, &dest->datatouch->x);
                break;
            case YCHECK:
                copycoordinate(&src->datatouch->y, &dest->datatouch->y);
                break;
            case PRESSURE:
                copypressure(&src->datatouch->pressure, &dest->datatouch->pressure);
                break;
            case IDTRATTO:
                copyidtratto(&src->datatouch->idtratto, &dest->datatouch->idtratto);
                break;
        }
    }
}

/* TODO -> understand what has changed */

static void copycolor(QList<struct colore_s> *src, QList<struct colore_s> *dest){
    int i, len;
    len = src->length();

    dest->clear();

    for(i=0; i<len; i++){
        dest->append(src[i]);
    }
}

static void copycoordinate(QList<double> *src, QList<double> *dest){
    int i, len;
    len = src->length();

    dest->clear();

    for(i=0; i<len; i++){
        dest->append(src[i]);
    }
}

static void copypressure(QList<float> *src, QList<float> *dest){
    int i, len;
    len = src->length();

    dest->clear();
    for(i=0; i<len; i++){
        dest->append(src[i]);
    }
}

static void copyidtratto(QList<int> *src, QList<int> *dest){
    int i, len;
    len = src->length();

    dest->clear();

    for(i=0; i<len; i++){
        dest->append(src[i]);
    }
}
