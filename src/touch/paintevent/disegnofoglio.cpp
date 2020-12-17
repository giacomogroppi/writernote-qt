#include "../tabletcanvas.h"

#include <QDebug>

void TabletCanvas::disegnafoglio(){
    if(!disegnofoglio_bool) return;
    qDebug() << "Disegno foglio";
    int last = this->data->last();
    int i, j, len = this->m_pixmap.width();


    for(j=0; j < 2; j++){
        for(i=0; i < len; i++){
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
        last += 10;
        len --;

        this->data->x.append(0);
        this->data->y.append(last);
        this->data->color.append(Qt::blue);
        this->data->posizioneaudio.append(0);

        this->data->pressure.append(0.01);
        this->data->rotation.append(0);

        this->data->idtratto.append(data->idtratto.last()+1);
    }

    this->disegnofoglio_bool = false;
    this->data->numeropagine += 1;
}
