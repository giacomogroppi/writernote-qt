#include "touch/tabletcanvas.h"
#include <QEvent>
#include "math.h"
#include <QWheelEvent>
#include <QDebug>
#include "mainwindow.h"

static inline double Distance(const QPointF &point1, const QPointF &point2)
{
    double x, y;
    x = pow(point1.x() - point2.x(), 2);
    y = pow(point1.y() - point2.y(), 2);
    return x + y;
}

static force_inline QPointF PointMiddle(const QPointF &first, const QPointF &sec)
{
    double x, y;
    x = (first.x() + sec.x()) / 2.0;
    y = (first.y() + sec.y()) / 2.0;

    return QPointF(x, y);
}

extern bool isZooming;

#define ISDEFINE_RIGHT(x) x[1].set
#define ISDEFINE_LEFT(x) x[0].set
#define ISDEFINE(x) (x[1].set)&&(x[0].set)
#define RIDEFINE(x) x[0].set = x[1].set = false;

bool block_scrolling = false;

bool TabletCanvas::event(QEvent *event)
{
    constexpr bool TabletEventDebug = true;

    bool needToResize = false;
    QPointF tmp, _pointMiddle;
    double tmp_distance, tmp_distance_right_left;

    const QSize maxSize = this->size();
    const QSize size = this->_pixmap.size();

    bool zoomChange = false;
    const auto type = event->type();
    const auto isTouchEvent =   type == QEvent::TouchEnd ||
                                type == QEvent::TouchBegin ||
                                type == QEvent::TouchUpdate;

    //WDebug(TabletEventDebug, "TabletCanvas" << __FUNCTION__ << event->type());

    if(type == QEvent::TouchEnd){
        WDebug(TabletEventDebug, __func__ << "Ridefine");
        RIDEFINE(this->lastpointzoom);
        block_scrolling = false;
        isZooming = false;
        return QWidget::event(event);
    }

    if(isTouchEvent){
        const QList<QTouchEvent::TouchPoint> touchPoints = static_cast<QTouchEvent *>(event)->touchPoints();

        // se l'utente sta zoomando con le dita touchPoints ha per forza lunghezza due.
        if(unknown(touchPoints.length() != 2)){
            goto out;
        }

        //WDebug(TabletEventDebug, __func__ << touchPoints.length());
        //return QWidget::event(event);

        for( const auto &touchPoint : touchPoints){
            cbool _tmp =    touchPoint.state() == Qt::TouchPointPressed ||
                            touchPoint.state() == Qt::TouchPointMoved ||
                            touchPoint.state() == Qt::TouchPointStationary;
            if(_tmp){
                block_scrolling = true;
                const QPointF &pointTouch = touchPoint.pos();
                if(ISDEFINE(lastpointzoom)){
                    /* si calcola la distanza tra il punto dell'ultimo touch e il punto corrente, e si usa il più lontano */
                    const auto Dist1 = Distance(lastpointzoom[0].point, pointTouch);
                    const auto Dist2 = Distance(this->lastpointzoom[1].point, pointTouch);
                    const int FirstMoreDistance =  Dist1 > Dist2;
                    const QPointF &PointSelected = (FirstMoreDistance) ? lastpointzoom[0].point : lastpointzoom[1].point;
                    const double distanceSelected = Distance(PointSelected, pointTouch);

                    _pointMiddle = PointMiddle(lastpointzoom[0].point, pointTouch);

                    tmp_distance_right_left = Distance(lastpointzoom[0].point, lastpointzoom[1].point);

                    tmp_distance = distanceSelected / tmp_distance_right_left;

                    //qDebug() <<tmpNum<< "tmp_distance " << tmp_distance << "Distance_rigth_ledt" << tmp_distance_right_left << "Distance(PointSelected, pointTouch)" << distanceSelected;

                    needToResize = needToResize || _zoom->zoom(_pointMiddle, tmp_distance, zoomChange, size, maxSize, data->datatouch);

                    //lastpointzoom[0].set = lastpointzoom[1].set = false;
                    lastpointzoom[FirstMoreDistance].point = pointTouch;

                    //qDebug() << tmpNum << "Index for update data" << int(FirstMoreDistance);
                }
                else{
                    const uchar index = !lastpointzoom[0].isNotSet();
                    lastpointzoom[index].set = true;
                    lastpointzoom[index].point = pointTouch;
                    qDebug() << "Index: " << index;
                    //continue;

                    if(!this->lastpointzoom[1].set){
                        this->lastpointzoom[1].point = touchPoint.pos();
                        this->lastpointzoom[1].set = true;
                    }
                    else{
                        if(touchPoint.pos().x() > this->lastpointzoom[1].point.x()){
                            /* deve fare lo scambio */
                            tmp = this->lastpointzoom[1].point;
                            this->lastpointzoom[1].point = touchPoint.pos();

                            this->lastpointzoom[0].point = tmp;
                            this->lastpointzoom[0].set = true;
                        }
                        else{
                            /* altrimenti vuol dire che il punto a destra è già quello a destra */
                            this->lastpointzoom[0].point = touchPoint.pos();
                        }
                    }
                }

            }
        } // for touchPoints
    }
    else{
        return QWidget::event(event);
    }

    //qDebug() << "After" << this->lastpointzoom[0].point << this->lastpointzoom[1].point << lastpointzoom[0].set << lastpointzoom[1].set << "\n";
    if(needToResize)
        this->callResizeEvent();

    if(zoomChange)
        _parent->zoomChange();

out:
    update();
    return QWidget::event(event);
}
