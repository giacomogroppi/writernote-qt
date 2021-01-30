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

    QColor coloretemp(Qt::blue);
    struct colore_s coloretemp_struttura;

    /* insert a point (0, 0) */
    if(this->data->x.length() == 0){
        this->data->x.append(0);
        this->data->y.append(0);
        this->data->idtratto.append(-5);

        /* no matter what color it is, it is not drawn */
        this->data->color.append(coloretemp_struttura);
        this->data->pressure.append(0);
        this->data->rotation.append(0);
        this->data->posizioneaudio.append(0);
    }

    coloretemp.getRgb(&coloretemp_struttura.colore[0],
            &coloretemp_struttura.colore[1],
            &coloretemp_struttura.colore[2],
            &coloretemp_struttura.colore[3]);

    for(i = 0, last += 10; i < 61; i++, last += 40){
        /* punto inizio riga */
        this->data->x.append(20.00);
        this->data->y.append((double)last);
        this->data->color.append(coloretemp_struttura);
        this->data->posizioneaudio.append(0);

        this->data->pressure.append(0.001);
        this->data->rotation.append(0);

        this->data->idtratto.append(-1);

        /* punto di fine della riga */

        this->data->x.append((double)(lunghezza - 20));
        this->data->y.append((double)last);
        this->data->color.append(coloretemp_struttura);
        this->data->posizioneaudio.append(0);

        this->data->pressure.append(0.001);
        this->data->rotation.append(0);

        this->data->idtratto.append(-1);

    }

    /* punto inizio riga */
    this->data->x.append(0.0);
    this->data->y.append((double)last);
    this->data->color.append(coloretemp_struttura);
    this->data->posizioneaudio.append(0);

    this->data->pressure.append(0.001);
    this->data->rotation.append(0);

    this->data->idtratto.append(-1);

    /* punto di fine della riga */
    this->data->x.append((double)lunghezza);
    this->data->y.append((double)last);
    this->data->color.append(coloretemp_struttura);
    this->data->posizioneaudio.append(0);

    this->data->pressure.append(0.001);
    this->data->rotation.append(0);

    this->data->idtratto.append(-1);


    this->disegnofoglio_bool = false;
    this->isloading = true;
}
