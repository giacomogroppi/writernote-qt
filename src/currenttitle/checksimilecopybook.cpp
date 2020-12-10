#include "checksimilecopybook.h"

#include "currenttitle_class.h"
#include "../indice_class.h"

#include <QDebug>

/* la funzione confronta due oggetti della classe current title */
bool checksimilecopybook(currenttitle_class *primo, currenttitle_class *secondo)
{
    if(primo->testi != secondo->testi)
        return false;


    if(primo->testinohtml.length() != secondo->testinohtml.length())
        return false;


    int i, len = primo->testinohtml.length();
    for(i=0; i < len; i++ )
        if(primo->testinohtml.at(i) != secondo->testinohtml.at(i))
            return false;


    if(primo->audio_position_path != secondo->audio_position_path)
        return false;

    return true;
}

bool checksimileindice(indice_class *primo, indice_class *secondo){
    int i, len;

    if(primo->audio.length() != secondo->audio.length())
        return false;

    len = primo->audio.length();
    for(i=0; i < len; i++)
        if(primo->audio.at(i) != secondo->audio.at(i))
            return false;

    if(primo->compressione.length() != secondo->compressione.length())
        return false;

    len = primo->compressione.length();
    for(i=0; i < len; i++)
        if(primo->compressione.at(i) != secondo->compressione.at(i))
            return false;

    if(primo->titolo.length() != secondo->titolo.length())
        return false;

    len = primo->titolo.length();
    for(i=0; i < len; i++)
        if(primo->titolo.at(i) != secondo->titolo.at(i))
            return false;


    if(primo->video.length() != secondo->video.length())
        return false;

    len = primo->video.length();
    for(i=0; i < len; i++)
        if(primo->video.at(i) != secondo->video.at(i))
            return false;

    return true;
}
