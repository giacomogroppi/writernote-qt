#include "tabletcanvas.h"

#include <QTabletEvent>
#include <QPainter>

#define MAXPOINT 20

static bool neet_to_change_color(datastruct *data, int id){
    int i, len, how;

    len = data->m_point.length();
    //len = data->x.length();

    for(i=0, how = 0; i<len; i++){
        if(data->m_point.at(i).idtratto == id)
            how ++;
    }

    /*
     * last data struct
    */
    /*
    for(i=0, how = 0; i<len; i++){
        if(data->idtratto[i] == id){
            how ++;
        }
    }*/
    return how == MAXPOINT;
}

void TabletCanvas::updatelist(QTabletEvent *event){
    struct point_s temp_point;

    if(!this->m_deviceDown){
        if(this->data->datatouch->m_point.length())
            data->datatouch->m_point.append();
    }






    if(!this->m_deviceDown){
        if(this->data->datatouch->idtratto.length())
            this->data->datatouch->idtratto.append(this->data->datatouch->idtratto.last() + 1);
        else
            this->data->datatouch->idtratto.append(0);
    }
    else{
        if(this->m_pen_ui->m_type_tratto == TRATTI){
            if(neet_to_change_color(data->datatouch, data->datatouch->idtratto.last())){
                if(m_pen_ui->m_last_color.ok == false){

                    /* save the current color */
                    m_pen_ui->m_last_color.ok = true;
                    m_pen_ui->m_last_color.color = m_color;

                    /* he need to change color into white */
                    this->m_color = Qt::white;
                }
                else{
                    /* restore the last color */
                    m_pen_ui->m_last_color.ok = false;

                    this->m_color = m_pen_ui->m_last_color.color;
                }
            }
        }

        this->data->datatouch->idtratto.append(data->datatouch->idtratto.last());
    }

    struct colore_s colore;

    this->data->datatouch->x.append(event->posF().x());
    this->data->datatouch->y.append(event->posF().y());

    this->data->datatouch->pressure.append(event->pressure());
    this->data->datatouch->rotation.append(event->rotation());

    this->data->datatouch->posizioneaudio.append(this->time/1000);

    m_color.getRgb(&colore.colore[0],
            &colore.colore[1],
            &colore.colore[2],
            &colore.colore[3]);

    this->data->datatouch->color.append(colore);
}


/* la funzioen gestisce tutti i pixel */
void TabletCanvas::paintPixmap(QPainter &painter, QTabletEvent *event){
    static qreal maxPenradius = pressureToWidth(1.0);
    painter.setRenderHint(QPainter::Antialiasing);

    painter.setPen(this->m_pen);
    painter.drawLine(lastPoint.pos, event->posF());
    update(QRect(lastPoint.pos.toPoint(), event->pos()).normalized()
           .adjusted(-maxPenradius, -maxPenradius, maxPenradius, maxPenradius));

}
