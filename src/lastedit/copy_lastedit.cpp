#include "redoundo.h"
#include "../currenttitle/checksimilecopybook.h"

static inline void copycolor(const datastruct *, datastruct *);

static inline void copycoordinatey(const datastruct *, datastruct *);
static inline void copycoordinatex(const datastruct *, datastruct *);

static inline void copypressure(const datastruct *, datastruct *);
static inline void copyidtratto(const datastruct *, datastruct *);

static inline void copyall(const datastruct *, datastruct *);

void redoundo::copy_b(currenttitle_class *dest, const currenttitle_class *src){
    int temp = COLORE;
    bool ok;

    do{
        temp = checksimilecopybook(src, dest, true);
        ok = false;

        switch (temp) {
            case LEN:
                copyall(src->datatouch, dest->datatouch);
                break;

            case COLORE:
                copycolor(src->datatouch, dest->datatouch);
                ok = true;
                break;

            case XCHECK:
                copycoordinatex(src->datatouch, dest->datatouch);
                ok = true;
                break;

            case YCHECK:
                copycoordinatey(src->datatouch, dest->datatouch);
                ok = true;
                break;

            case PRESSURE:
                copypressure(src->datatouch, dest->datatouch);
                ok = true;
                break;

            case IDTRATTO:
                copyidtratto(src->datatouch, dest->datatouch);
                ok = true;
                break;

            case LEN_POSIZIONEFOGLIO:
            {
                dest->datatouch->posizionefoglio = src->datatouch->posizionefoglio;

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


void redoundo::copy(currenttitle_class *m_temp){
    return;

    if(indice < QUANTIINDICI){
        m_list.append();

        copy_b(m_list.m_list[indice], m_temp);
        this->indice ++;
        return;
    }

    currenttitle_class * title_temp = m_list.m_list[0];

    /* must translate all objects to the left by one position */
    for(int i=0; i<QUANTIINDICI-1; i++){
        m_list.m_list[i] = m_list.m_list[i+1];
    }

    m_list.m_list[QUANTIINDICI-1] = title_temp;

    copy_b(m_list.m_list[QUANTIINDICI-1], m_temp);
}

static inline void copycolor(const datastruct *src,
                             datastruct *dest){
    uint i, len = src->length();

    for(i=0; i<len; ++i){
        memcpy((point_s *)dest->at(i), src->at(i), datastruct::getSizeOne());
    }

/*    for(i=0; i<len; i++){
        memcpy(&dest->operator[](i).m_color, &src->operator[](i).m_color, sizeof(colore_s));
    }*/
}

static inline void copycoordinatey(const datastruct *src,
                                   datastruct *dest){
    uint i, len;
    len = src->length();

    for(i=0; i<len; i++){
        dest->at_mod(i)->m_y = src->at(i)->m_y;
    }
}

static inline void copycoordinatex(const datastruct *src,
                                   datastruct *dest){
    uint i, len;

    len = src->length();

    for(i=0; i<len; ++i){
        dest->at_mod(i)->m_x = src->at(i)->m_x;
    }
}

static inline void copypressure( const datastruct *src,
                                 datastruct *dest){
    int i, len;
    len = src->length();

    for(i=0; i<len; i++){
        dest->at_mod(i)->m_pressure = src->at(i)->m_pressure;
    }
}

static inline void copyidtratto(const datastruct *src,
                                datastruct *dest){
    int i, len;
    len = src->length();

    for(i=0; i<len; i++){
        dest->at_mod(i)->idtratto = src->at(i)->idtratto;
    }
}

static inline void copyall(const datastruct *src,
                           datastruct *dest){
    dest->operator=(*src);
}



