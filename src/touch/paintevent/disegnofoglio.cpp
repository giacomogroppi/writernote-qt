#include "../tabletcanvas.h"
#include <QDebug>

void TabletCanvas::disegnafoglio(){
    if(!disegnofoglio_bool) return;
    qDebug() << "Disegno foglio richiamata";

    int i, last = 0, temp;
    int lunghezza = this->m_pixmap.width();

    if(data->posizionefoglio.length())
       last = data->posizionefoglio.last() + NUMEROPIXELPAGINA;

    temp = last;

    for(i = 0, last += 10; i < 61; i++, last += 20){
        /* punto inizio riga */
        this->data->x.append(20);
        this->data->y.append(last);
        this->data->color.append(Qt::blue);
        this->data->posizioneaudio.append(0);

        this->data->pressure.append(0.001);
        this->data->rotation.append(0);

        this->data->idtratto.append(-1);

        /* punto di fine della riga */

        this->data->x.append(lunghezza - 20);
        this->data->y.append(last);
        this->data->color.append(Qt::blue);
        this->data->posizioneaudio.append(0);

        this->data->pressure.append(0.001);
        this->data->rotation.append(0);

        this->data->idtratto.append(-1);

    }

    /* punto inizio riga */
    this->data->x.append(0);
    this->data->y.append(last);
    this->data->color.append(Qt::blue);
    this->data->posizioneaudio.append(0);

    this->data->pressure.append(0.001);
    this->data->rotation.append(0);

    this->data->idtratto.append(-1);

    /* punto di fine della riga */
    this->data->x.append(lunghezza);
    this->data->y.append(last);
    this->data->color.append(Qt::blue);
    this->data->posizioneaudio.append(0);

    this->data->pressure.append(0.001);
    this->data->rotation.append(0);

    this->data->idtratto.append(-1);

    data->posizionefoglio.append(temp);

    this->disegnofoglio_bool = false;
    this->isloading = true;
    update();
}
