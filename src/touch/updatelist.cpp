#include "tabletcanvas.h"

#include <QTabletEvent>
#include <QPainter>

#define MAXPOINT 20
#include "../utils/color/setcolor.h"

static bool need_to_change_color(datastruct *data, int id){
    int i, len, how;

    len = data->m_point.length();

    for(i=0, how = 0; i<len; i++){
        if(data->m_point.at(i).idtratto == id)
            how ++;
    }

    if(!how)
        return false;

    return (how % MAXPOINT) ? 0 : 1;
}

void TabletCanvas::updatelist(QTabletEvent *event){
    struct point_s temp_point;

    if(!this->m_deviceDown){
        if(this->data->datatouch->m_point.length())
            temp_point.idtratto = data->datatouch->m_point.last().idtratto + 1;
        else
            temp_point.idtratto = 0;
    }
    else{
        if(m_pen_ui->m_type_tratto == pen_ui::n_tratto::tratti){
            if(need_to_change_color(data->datatouch, data->datatouch->m_point.last().idtratto)){
                if(m_pen_ui->m_last_color.ok == false){
                    /* save the current color */

                    m_pen_ui->m_last_color.ok = true;
                    m_pen_ui->m_last_color.color = m_color;

                    this->m_color = Qt::white;

                }
                else{
                    /* restore the last color */
                    m_pen_ui->m_last_color.ok = false;
                    this->m_color = m_pen_ui->m_last_color.color;

                }
            }
        }

        temp_point.idtratto = data->datatouch->m_point.last().idtratto;
    }


    struct colore_s colore;

    temp_point.m_x = event->posF().x();
    temp_point.m_y = event->posF().y();
    temp_point.m_pressure = event->pressure();
    temp_point.rotation = event->rotation();

    temp_point.m_posizioneaudio = time/1000;


    setcolor_struct(&colore, m_color);

    memcpy(&temp_point.m_color, &colore, sizeof(colore_s));

    data->datatouch->m_point.append(temp_point);
}


/*
 * the function is called from TabletCanvas::tabletEvent
 * and print the data in the painter
*/
void TabletCanvas::paintPixmap(QPainter &painter, QTabletEvent *event){
    static qreal maxPenradius = pressureToWidth(1.0);
    painter.setRenderHint(QPainter::Antialiasing);

    painter.setPen(this->m_pen);
    painter.drawLine(lastPoint.pos, event->posF());
    update(QRect(lastPoint.pos.toPoint(), event->pos()).normalized()
           .adjusted(-maxPenradius, -maxPenradius, maxPenradius, maxPenradius));



}
