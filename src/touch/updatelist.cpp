#include "tabletcanvas.h"

#include <QTabletEvent>
#include <QPainter>
#include <QDebug>
#define MAXPOINT 20
#include "../utils/color/setcolor.h"

static bool need_to_change_color(datastruct *data, int id){
    static uint i, len, how, counterPage;
    static uint lenPage;
    lenPage = data->lengthPage();

    for(counterPage = 0; counterPage < lenPage; counterPage ++){
        len = data->at(counterPage)->length();
        for(i=0, how = 0; i<len; i++){
            if(data->at(i, counterPage)->idtratto == id)
                how ++;
        }
    }

    if(!how)
        return false;

    return (how % MAXPOINT) ? 0 : 1;
}

void TabletCanvas::updatelist(QTabletEvent *event){
    static double size;
    static uchar alfa;
    static point_s tmp_point;
    static QPointF PointFirstPage;

    const bool hightlighter = (medotodiinserimento == e_method::highlighter);
    const bool pen = (medotodiinserimento == e_method::pen);
    const QPointF &pointTouch = event->posF();

    PointFirstPage = this->data->datatouch->getPointFirstPage();

    size = event->pressure();
    alfa = hightlighter ? m_highlighter->getAlfa() : 255;

    if(!this->m_deviceDown){
        tmp_point.idtratto = (!data->datatouch->isempty()) ? data->datatouch->maxId() + 1 : 0;
    }
    else{
        if(pen && m_pen_ui->m_type_tratto == pen_ui::n_tratto::tratti){
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
        tmp_point.idtratto = data->datatouch->lastId();
    }

    tmp_point.m_x = pointTouch.x() - PointFirstPage.x();
    tmp_point.m_y = pointTouch.y() - PointFirstPage.y();
    tmp_point.m_pressure = hightlighter ? m_highlighter->getSize(size) : m_pen_ui->getSize(size);
    tmp_point.rotation = event->rotation();
    tmp_point.m_posizioneaudio = time/1000;

    setcolor_struct(&tmp_point.m_color, m_color);

    if(alfa != 255){
        qDebug() << "alfa: " << alfa;
    }

    tmp_point.m_color.colore[3] = alfa;

    //qDebug() << tmp_point.m_x << tmp_point.m_y << PointFirstPage << pointTouch;

    data->datatouch->append(tmp_point);
}


/*
 * the function is called from TabletCanvas::tabletEvent
 * and print the data in the painter
*/
void TabletCanvas::paintPixmap(QPainter &painter, QTabletEvent *event){
    Q_UNUSED(painter);
    Q_UNUSED(event);

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
