#include "checksimilecopybook.h"

#include "document.h"
#include "../indice_class.h"

#include <stdio.h>
#include <string.h>

#define P(x) x->datatouch

static int checkSpeed(const Document &first,
                      const Document &second){
    uint i, counterPage;
    const uint lenPage = first.datatouch->lengthPage();
    uint len;

    if(lenPage != second.datatouch->lengthPage())
        return LEN;

    for(counterPage = 0; counterPage < lenPage; counterPage ++){
        len = first.datatouch->at(counterPage)->length();
        if(len != second.datatouch->at(counterPage)->length())
            return LEN;

        for(i=0; i<len; i++){
            if(memcmp(first.datatouch->at(i),
                      second.datatouch->at(i),
                      datastruct::getSizeOne()) != 0)
                return IDTRATTO;
        }
    }

    return OK_CHECK;
}

static int checkSlow(const Document *first,
                     const Document *second){
    uint i, counterPage;
    const uint lenPage = first->datatouch->lengthPage();

    if(lenPage != P(second)->lengthPage())
        return LEN;

    for(counterPage = 0; counterPage < lenPage; counterPage ++){
        const uint len = first->datatouch->at(counterPage)->length();
        if(len != second->datatouch->at(counterPage)->length())
            return LEN;

        for(i=0; i<len; i++){
            const point_s *point1 = first->datatouch->at(i, counterPage);
            const point_s *point2 = second->datatouch->at(i, counterPage);

            if(point1->idtratto != point2->idtratto)
                return IDTRATTO;
            if(point1->m_x != point2->m_x)
                return XCHECK;
            if(point1->m_y != point2->m_y)
                return YCHECK;

            if(memcmp(&point1->m_color,
                      &point2->m_color,
                      sizeof(point1->m_color)) != 0)
                return COLORE;

            if(point1->m_posizioneaudio != point2->m_posizioneaudio)
                return AUDIOPOSITION;

            if(point1->m_pressure != point2->m_pressure)
                return PRESSURE;

            if(point1->rotation != point2->rotation)
                return ROTATION;
        }
    }
    return OK_CHECK;
}


/*
 * speed = true return checkSpeed, it do a memcpm
 * it's a much faster function if you don't care
 * what the difference is
 */

int checksimilecopybook(const Document &primo,
                        const Document &secondo,
                        const bool speed)
{
    int res;
    if(primo.count_img != secondo.count_img)
        return LEN;
    if(primo.count_pdf != secondo.count_pdf)
        return LEN;

    primo.datatouch->scala_all();
    secondo.datatouch->scala_all();

    if(speed){
        res = checkSpeed(primo, secondo);

        primo.datatouch->restoreLastTranslation();
        secondo.datatouch->restoreLastTranslation();

        return res;
    }

    res = checkSlow(&primo, &secondo);


    primo.datatouch->restoreLastTranslation();

    secondo.datatouch->restoreLastTranslation();

    return res;
}
