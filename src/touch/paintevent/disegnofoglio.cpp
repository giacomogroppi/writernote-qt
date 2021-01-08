#include "../tabletcanvas.h"
#include <QDebug>

void TabletCanvas::disegnafoglio(){
    if(!disegnofoglio_bool) return;

    int i, last = 0;
    int lunghezza;

    /*lunghezza = this->m_pixmap.width();*/
    lunghezza = NUMEROPIXELPAGINA * data->zoom;

    if(data->posizionefoglio.length())
       last = data->posizionefoglio.last() + NUMEROPIXELPAGINA * data->zoom;

    data->posizionefoglio.append(last);

    for(i = 0, last += 10; i < 61; i++, last += 40){
        /* punto inizio riga */
        this->data->x.append(20.00);
        this->data->y.append((double)last);
        this->data->color.append(Qt::blue);
        this->data->posizioneaudio.append(0);

        this->data->pressure.append(0.001);
        this->data->rotation.append(0);

        this->data->idtratto.append(-1);

        /* punto di fine della riga */

        this->data->x.append((double)(lunghezza - 20));
        this->data->y.append((double)last);
        this->data->color.append(Qt::blue);
        this->data->posizioneaudio.append(0);

        this->data->pressure.append(0.001);
        this->data->rotation.append(0);

        this->data->idtratto.append(-1);

    }

    /* punto inizio riga */
    this->data->x.append(0.0);
    this->data->y.append((double)last);
    this->data->color.append(Qt::blue);
    this->data->posizioneaudio.append(0);

    this->data->pressure.append(0.001);
    this->data->rotation.append(0);

    this->data->idtratto.append(-1);

    /* punto di fine della riga */
    this->data->x.append((double)lunghezza);
    this->data->y.append((double)last);
    this->data->color.append(Qt::blue);
    this->data->posizioneaudio.append(0);

    this->data->pressure.append(0.001);
    this->data->rotation.append(0);

    this->data->idtratto.append(-1);


    this->disegnofoglio_bool = false;
    this->isloading = true;
}
