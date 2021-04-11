#include "checksimilecopybook.h"

#include "currenttitle_class.h"
#include "../indice_class.h"

#include <stdio.h>
#include <string.h>

#define P(x) x->datatouch->m_point

static int checkPositionAudio(currenttitle_class *first, currenttitle_class *second);

static int checkIndiceSlow(currenttitle_class *primo, currenttitle_class * secondo){
    if(primo->testi != secondo->testi)
        return TESTI;

    if(primo->testinohtml.length() != secondo->testinohtml.length())
        return TESTINOHTML;

    int i, len = primo->testinohtml.length();
    for(i=0; i < len; i++ )
        if(primo->testinohtml.at(i) != secondo->testinohtml.at(i))
            return TESTINOHTML;

    if(primo->audio_position_path != secondo->audio_position_path)
        return AUDIOPOSITION;

    return OK_CHECK;
}

static int checkSpeed(currenttitle_class *first, currenttitle_class *second){
    int i, len;

    len = first->datatouch->m_point.length();
    if(len != second->datatouch->m_point.length())
        return IDTRATTO;

    for(i=0; i<len; i++){
        if(memcmp(&P(first).operator[](i),
                  &P(second).operator[](i),
                  sizeof(point_s)) != 0)
            return IDTRATTO;
    }

    return checkPositionAudio(first, second);
}

static int checkSlow(currenttitle_class *first, currenttitle_class *second){
    int i, len;

    len = P(first).length();
    if(len != P(second).length())
        return LEN;

    for(i=0; i<len; i++){
        if(P(first).at(i).idtratto != P(second).at(i).idtratto)
            return IDTRATTO;
        if(P(first).at(i).m_x != P(second).at(i).m_x)
            return XCHECK;
        if(P(first).at(i).m_y != P(second).at(i).m_y)
            return YCHECK;

        if(memcmp(&P(first).operator[](i).m_color,
                  &P(second).operator[](i).m_color,
                  sizeof(struct colore_s)) != 0)
            return COLORE;

        if(P(first).at(i).m_posizioneaudio != P(second).at(i).m_posizioneaudio)
            return AUDIOPOSITION;

        if(P(first).at(i).m_pressure != P(second).at(i).m_pressure)
            return PRESSURE;

        if(P(first).at(i).rotation != P(second).at(i).rotation)
            return ROTATION;
    }
    return OK_CHECK;
}


/*
 * speed = true return checkSpeed, it do a memcpm
 * it's a much faster function if you don't care
 * what the difference is
 */

int checksimilecopybook(currenttitle_class *primo, currenttitle_class *secondo, bool speed)
{
    if(!primo->m_touch)
        return checkIndiceSlow(primo, secondo);

    int res;

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

int checksimileindice(indice_class *primo, indice_class *secondo){
    int i, len;

    if(primo->titolo.length() != secondo->titolo.length())
        return LEN;

    len = primo->titolo.length();
    for(i=0; i < len; i++)
        if(primo->titolo.at(i) != secondo->titolo.at(i))
            return LEN;

    return OK_CHECK;
}

static int checkPositionAudio(currenttitle_class *first, currenttitle_class *second){
    int len = first->datatouch->posizionefoglio.length(), i;

    if(second->datatouch->posizionefoglio.length() != len)
        return LEN_POSIZIONEFOGLIO;

    for(i=0; i<len; i++){
        if(first->datatouch->posizionefoglio.at(i) != second->datatouch->posizionefoglio.at(i))
            return POSIZIONE_FOGLIO;
    }

    return OK_CHECK;
}
