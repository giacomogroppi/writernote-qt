#include "checksimilecopybook.h"

#include "currenttitle_class.h"
#include "../indice_class.h"

#include <stdio.h>
#include <string.h>

#define P(x) x->datatouch->m_point

/* la funzione confronta due oggetti della classe current title */
int checksimilecopybook(currenttitle_class *primo, currenttitle_class *secondo)
{
    if(!primo->m_touch){
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


    }else{
        int i, len;

        len = P(primo).length();
        if(len != P(secondo).length())
            return LEN;

        for(i=0; i<len; i++){
            if(P(primo).at(i).idtratto != P(secondo).at(i).idtratto)
                return IDTRATTO;
            if(P(primo).at(i).m_x != P(secondo).at(i).m_x)
                return XCHECK;
            if(P(primo).at(i).m_y != P(secondo).at(i).m_y)
                return YCHECK;

            if(memcmp(&P(primo).at(i).m_color,
                      &P(secondo).at(i).m_color,
                      sizeof(struct colore_s)) != 0)
                return COLORE;

            if(P(primo).at(i).m_posizioneaudio != P(secondo).at(i).m_posizioneaudio)
                return AUDIOPOSITION;

            if(P(primo).at(i).m_pressure != P(secondo).at(i).m_pressure)
                return PRESSURE;

            if(P(primo).at(i).rotation != P(secondo).at(i).rotation)
                return ROTATION;
        }

        /*
         * last data struct
        */
        /*
        len = primo->datatouch->x.length();
        if(len != secondo->datatouch->x.length())
            return XCHECK;

        for(i = 0; i < len; i++)
            if(primo->datatouch->x.at(i) != secondo->datatouch->x.at(i))
                return XCHECK;

        len = primo->datatouch->y.length();
        if(len != secondo->datatouch->y.length())
            return YCHECK;
        for(i = 0; i < len; i++)
            if(primo->datatouch->y.at(i) != secondo->datatouch->y.at(i))
                return YCHECK;


        len = primo->datatouch->idtratto.length();
        if(len != secondo->datatouch->idtratto.length())
            return IDTRATTO;
        for(i=0; i<len; i++)
            if(primo->datatouch->idtratto.at(i) != secondo->datatouch->idtratto.at(i))
                return IDTRATTO;

        len = primo->datatouch->color.length();
        if(len != secondo->datatouch->color.length())
            return COLORE;

        int k;
        for(i=0; i<len; i++)
            for(k=0; k<4; k++)
                if(primo->datatouch->color.at(i).colore[k] != secondo->datatouch->color.at(i).colore[k])
                    return COLORE;



        len = primo->datatouch->pressure.length();
        if(len != secondo->datatouch->pressure.length())
            return PRESSURE;
        for(i=0; i<len; i++)
            if(primo->datatouch->pressure.at(i) != secondo->datatouch->pressure.at(i))
                return PRESSURE;


        len = primo->datatouch->rotation.length();
        if(len != secondo->datatouch->rotation.length())
            return ROTATION;
        for(i=0; i<len; i++)
            if(primo->datatouch->rotation.at(i) != secondo->datatouch->rotation.at(i))
                return ROTATION;*/

    }

    return OK_CHECK;
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
