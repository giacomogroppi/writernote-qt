#include "../tabletcanvas.h"
#include <QDebug>

void TabletCanvas::disegnafoglio(){
    if(!disegnofoglio_bool) return;

    int i, last = 0;
    int lunghezza;

    /*lunghezza = this->m_pixmap.width();*/
    lunghezza = NUMEROPIXELPAGINA * data->datatouch->zoom;

    if(data->datatouch->posizionefoglio.length())
       last = data->datatouch->posizionefoglio.last() + NUMEROPIXELPAGINA * data->datatouch->zoom;

    data->datatouch->posizionefoglio.append(last);

    QColor coloretemp(Qt::blue);
    struct colore_s coloretemp_struttura;

    /* insert a point (0, 0) */
    if(this->data->datatouch->x.length() == 0){
        this->data->datatouch->x.append(0);
        this->data->datatouch->y.append(0);
        this->data->datatouch->idtratto.append(-5);

        /* no matter what color it is, it is not drawn */
        this->data->datatouch->color.append(coloretemp_struttura);
        this->data->datatouch->pressure.append(0);
        this->data->datatouch->rotation.append(0);
        this->data->datatouch->posizioneaudio.append(0);
    }

    coloretemp.getRgb(&coloretemp_struttura.colore[0],
            &coloretemp_struttura.colore[1],
            &coloretemp_struttura.colore[2],
            &coloretemp_struttura.colore[3]);

    for(i = 0, last += 10; i < 61; i++, last += 40){
        /* punto inizio riga */
        this->data->datatouch->x.append(20.00);
        this->data->datatouch->y.append((double)last);
        this->data->datatouch->color.append(coloretemp_struttura);
        this->data->datatouch->posizioneaudio.append(0);

        this->data->datatouch->pressure.append(0.001);
        this->data->datatouch->rotation.append(0);

        this->data->datatouch->idtratto.append(-1);

        /* punto di fine della riga */

        this->data->datatouch->x.append((double)(lunghezza - 20));
        this->data->datatouch->y.append((double)last);
        this->data->datatouch->color.append(coloretemp_struttura);
        this->data->datatouch->posizioneaudio.append(0);

        this->data->datatouch->pressure.append(0.001);
        this->data->datatouch->rotation.append(0);

        this->data->datatouch->idtratto.append(-1);

    }

    /* punto inizio riga */
    this->data->datatouch->x.append(0.0);
    this->data->datatouch->y.append((double)last);
    this->data->datatouch->color.append(coloretemp_struttura);
    this->data->datatouch->posizioneaudio.append(0);

    this->data->datatouch->pressure.append(0.001);
    this->data->datatouch->rotation.append(0);

    this->data->datatouch->idtratto.append(-1);

    /* punto di fine della riga */
    this->data->datatouch->x.append((double)lunghezza);
    this->data->datatouch->y.append((double)last);
    this->data->datatouch->color.append(coloretemp_struttura);
    this->data->datatouch->posizioneaudio.append(0);

    this->data->datatouch->pressure.append(0.001);
    this->data->datatouch->rotation.append(0);

    this->data->datatouch->idtratto.append(-1);


    this->disegnofoglio_bool = false;
    this->isloading = true;
}
