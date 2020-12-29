#include "checksimilecopybook.h"

#include "currenttitle_class.h"
#include "../indice_class.h"

#include <QDebug>

/* la funzione confronta due oggetti della classe current title */
bool checksimilecopybook(currenttitle_class *primo, currenttitle_class *secondo)
{
    if(primo->posizione_binario == ""){
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

    }else{
        int i, len;

        qDebug() << "x";

        len = primo->datatouch->x.length();
        if(len != secondo->datatouch->x.length())
            return false;

        qDebug() << "Len ok";

        for(i = 0; i < len; i++)
            if(primo->datatouch->x.at(i) != secondo->datatouch->x.at(i))
                return false;

        qDebug() << "y";

        len = primo->datatouch->y.length();
        if(len != secondo->datatouch->y.length())
            return false;
        for(i = 0; i < len; i++)
            if(primo->datatouch->y.at(i) != secondo->datatouch->y.at(i))
                return false;

        qDebug() << "idtratto";

        len = primo->datatouch->idtratto.length();
        if(len != secondo->datatouch->idtratto.length())
            return false;
        for(i=0; i<len; i++)
            if(primo->datatouch->idtratto.at(i) != secondo->datatouch->idtratto.at(i))
                return false;

        //qDebug() << "colore";
        len = primo->datatouch->color.length();
        if(len != secondo->datatouch->color.length())
            return false;

        for(i=0; i<len; i++){
            if(primo->datatouch->color.at(i) != secondo->datatouch->color.at(i))
            {
                for(; i < len; i++)
                    qDebug() << primo->datatouch->color.at(i) << secondo->datatouch->color.at(i);
                return false;
            }
        }

        qDebug() << "pressure";

        len = primo->datatouch->pressure.length();
        if(len != secondo->datatouch->pressure.length())
            return false;
        for(i=0; i<len; i++)
            if(primo->datatouch->pressure.at(i) != secondo->datatouch->pressure.at(i))
                return false;

        qDebug() << "Rotation";

        len = primo->datatouch->rotation.length();
        if(len != secondo->datatouch->rotation.length())
            return false;
        for(i=0; i<len; i++)
            if(primo->datatouch->rotation.at(i) != secondo->datatouch->rotation.at(i))
                return false;
        qDebug() << "Fine";

    }

    return true;
}

bool checksimileindice(indice_class *primo, indice_class *secondo){
    int i, len;

    if(primo->titolo.length() != secondo->titolo.length())
        return false;

    len = primo->titolo.length();
    for(i=0; i < len; i++)
        if(primo->titolo.at(i) != secondo->titolo.at(i))
            return false;

    return true;
}
