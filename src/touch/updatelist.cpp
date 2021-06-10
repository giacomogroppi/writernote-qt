#include "tabletcanvas.h"

#include <QTabletEvent>
#include <QPainter>
#include <QDebug>
#define MAXPOINT 20
#include "../utils/color/setcolor.h"

static bool need_to_change_color(datastruct *data, int id){
    int i, len, how;

    len = data->length();

    for(i=0, how = 0; i<len; i++){
        if(data->at(i)->idtratto == id)
            how ++;
    }

    if(!how)
        return false;

    return (how % MAXPOINT) ? 0 : 1;
}

static point_s temp_point;

void TabletCanvas::updatelist(QTabletEvent *event){
    double size;
    uchar alfa;

    size = (m_pen_ui->m_type_pen == pen_ui::pressione) ? 0 : m_pen_ui->m_spessore_pen;

    if(medotodiinserimento == e_method::highlighter){
        size = 0;
        alfa = m_highlighter->getAlfa();
    }else{
        alfa = 255;
    }

    if(!this->m_deviceDown){
        temp_point.idtratto = (data->datatouch->length()) ? data->datatouch->maxId() + 1 : 0;
    }
    else{
        if(medotodiinserimento == e_method::pen
                && m_pen_ui->m_type_tratto == pen_ui::n_tratto::tratti){
            if(need_to_change_color(data->datatouch, data->datatouch->lastPoint()->idtratto)){
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

        temp_point.idtratto = data->datatouch->lastPoint()->idtratto;
    }

    temp_point.m_x = event->posF().x();
    temp_point.m_y = event->posF().y();

    if(!size)
        temp_point.m_pressure = event->pressure();
    else
        temp_point.m_pressure = m_pen_ui->m_spessore_pen;


    temp_point.rotation = event->rotation();

    temp_point.m_posizioneaudio = time/1000;

    setcolor_struct(&temp_point.m_color, m_color);

    if(alfa != 255){
        qDebug() << "alfa: " << alfa;
    }

    temp_point.m_color.colore[3] = alfa;

    data->datatouch->append(temp_point);
}


/*
 * the function is called from TabletCanvas::tabletEvent
 * and print the data in the painter
*/
void TabletCanvas::paintPixmap(QPainter &painter, QTabletEvent *event){
    isloading = true;
    update();
    return;

    /*
    static qreal maxPenradius = pressureToWidth(1.0);
    painter.setRenderHint(QPainter::Antialiasing);

    painter.setPen(this->m_pen);
    painter.drawLine(lastPoint.pos, event->posF());

    qreal press = event->pressure();

    update(QRect(lastPoint.pos.toPoint(), event->pos()).normalized()
           .adjusted(-maxPenradius, -maxPenradius, maxPenradius, maxPenradius));*/



}
