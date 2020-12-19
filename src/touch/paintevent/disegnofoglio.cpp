#include "../tabletcanvas.h"

#include <QDebug>

void TabletCanvas::disegnafoglio(){
    if(!disegnofoglio_bool) return;

    data->posizionefoglio.append(0);
    return;

    int i, last = 0;

    if(data->posizionefoglio.length()){
       last = data->posizionefoglio.last() + NUMEROPIXELPAGINA;
    }


    for(i=0; i < NUMEROPIXELORIZZONALI; i++){
        this->data->x.append(i);
        this->data->y.append(last);
        this->data->color.append(Qt::blue);
        this->data->posizioneaudio.append(0);

        this->data->pressure.append(0.01);
        this->data->rotation.append(0);

        if(data->idtratto.length())
            this->data->idtratto.append(data->idtratto.last());
        else
            this->data->idtratto.append(0);
    }

    data->posizionefoglio.append(last);

    this->disegnofoglio_bool = false;
}
