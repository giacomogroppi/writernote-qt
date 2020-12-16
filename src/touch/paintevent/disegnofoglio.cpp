#include "../tabletcanvas.h"

#include <QDebug>

void TabletCanvas::disegnafoglio(){
    if(!disegnofoglio_bool) return;
    qDebug() << "Disegno foglio";
    int last = this->data->last();
    int i, j;


    for(j=0; j < 2; j++){
        for(i=0; i < this->m_pixmap.width(); i++){
            this->data->x.append(i);
            this->data->y.append(last);
            this->data->color.append(Qt::blue);
            this->data->posizioneaudio.append(this->time);

            if(data->idtratto.length())
                this->data->idtratto.append(data->idtratto.last());
            else
                this->data->idtratto.append(0);
        }
        last += 10;
    }

    this->disegnofoglio_bool = false;
    this->data->numeropagine += 1;
}
