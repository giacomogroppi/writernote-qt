#include "checksimilecopybook.h"

#include "document.h"
#include "../indice_class.h"

#include <stdio.h>
#include <string.h>

#define P(x) x->datatouch

static int checkPositionAudio(const Document *first,
                              const Document *second);

static int checkSpeed(const Document *first,
                      const Document *second){
    uint i, len;

    len = first->datatouch->length();
    if(len != second->datatouch->length())
        return IDTRATTO;

    for(i=0; i<len; i++){
        if(memcmp(first->datatouch->at(i),
                  second->datatouch->at(i),
                  datastruct::getSizeOne()) != 0)
            return IDTRATTO;
    }

    return checkPositionAudio(first, second);
}

static int checkSlow(const Document *first,
                     const Document *second){
    uint i, len;

    len = P(first)->length();
    if(len != P(second)->length())
        return LEN;

    for(i=0; i<len; i++){
        if(P(first)->at(i)->idtratto != P(second)->at(i)->idtratto)
            return IDTRATTO;
        if(P(first)->at(i)->m_x != P(second)->at(i)->m_x)
            return XCHECK;
        if(P(first)->at(i)->m_y != P(second)->at(i)->m_y)
            return YCHECK;

        if(memcmp(&first->datatouch->at(i)->m_color,
                  &second->datatouch->at(i)->m_color,
                  sizeof(struct colore_s)) != 0)
            return COLORE;

        if(P(first)->at(i)->m_posizioneaudio != P(second)->at(i)->m_posizioneaudio)
            return AUDIOPOSITION;

        if(P(first)->at(i)->m_pressure != P(second)->at(i)->m_pressure)
            return PRESSURE;

        if(P(first)->at(i)->rotation != P(second)->at(i)->rotation)
            return ROTATION;
    }
    return OK_CHECK;
}


/*
 * speed = true return checkSpeed, it do a memcpm
 * it's a much faster function if you don't care
 * what the difference is
 */

int checksimilecopybook(const Document *primo,
                        const Document *secondo,
                        const bool speed)
{
    int res;
    if(primo->count_img != secondo->count_img)
        return LEN;
    if(primo->count_pdf != secondo->count_pdf)
        return LEN;

    primo->datatouch->scala_all();
    secondo->datatouch->scala_all();

    if(speed){
        res = checkSpeed(primo, secondo);

        primo->datatouch->restoreLastTranslation();
        secondo->datatouch->restoreLastTranslation();

        return res;
    }

    res = checkSlow(primo, secondo);


    primo->datatouch->restoreLastTranslation();

    secondo->datatouch->restoreLastTranslation();

    return res;
}

static int checkPositionAudio(const Document *first,
                              const Document *second){
    int len = first->datatouch->posizionefoglio.length(), i;

    if(second->datatouch->posizionefoglio.length() != len)
        return LEN_POSIZIONEFOGLIO;

    for(i=0; i<len; i++){
        if(first->datatouch->posizionefoglio.at(i) != second->datatouch->posizionefoglio.at(i))
            return POSIZIONE_FOGLIO;
    }

    return OK_CHECK;
}
