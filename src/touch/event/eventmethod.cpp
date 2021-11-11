#include "../tabletcanvas.h"
#include <QEvent>
#include "math.h"
#include <QWheelEvent>
#include <QDebug>
#include <cstdlib>

static inline double Distance(const QPointF &point1, const QPointF &point2);
static inline void scambio(QPointF &, QPointF &);
static inline QPointF puntoameta(const QPointF &, const QPointF &);

extern bool isZooming;

#define ISDEFINE_RIGHT(x) x[1].set
#define ISDEFINE_LEFT(x) x[0].set
#define ISDEFINE(x) (x[1].set)&&(x[0].set)
#define RIDEFINE(x) x[0].set = x[1].set = false;

/*
 * this function only manage zoom
*/

static QPointF pointMiddle;
static bool needToResize;

bool TabletCanvas::event(QEvent *event){
    needToResize = false;
    static QPointF tmp;
    static double tmp_distance;
    static double tmp_distance_right_left;

    const int maxHeight = this->height();
    const int maxWidth = this->width();

    const int heightPixmap = this->m_pixmap.height();
    const int widthPixmap = this->m_pixmap.width();

    const auto type = event->type();

    /*if(type == QEvent::TouchBegin || type == QEvent::TouchUpdate || type == QEvent::TouchEnd)
        qDebug() << " TabletCanvas::event " << type;*/

    //return QWidget::event(event);

    if(type == QEvent::TouchEnd){
        qDebug() << "Ridefine";
        RIDEFINE(this->lastpointzoom);
    }

    switch (type) {
        case QEvent::TouchBegin:
        case QEvent::TouchUpdate:
        case QEvent::TouchEnd:
        {
            const QList<QTouchEvent::TouchPoint> touchPoints = static_cast<QTouchEvent *>(event)->touchPoints();

            const size_t tmpNum = rand() % 90 + 10;

            for( const auto &touchPoint : touchPoints){
            //qDebug() << tmpNum << touchPoint.pos() << this->lastpointzoom[0].point << this->lastpointzoom[1].point << lastpointzoom[0].set << lastpointzoom[1].set;

            switch (touchPoint.state()) {
            case Qt::TouchPointStationary:
            case Qt::TouchPointReleased:
                continue;
            default:
            {
                const QPointF &pointTouch = touchPoint.pos();
                //qDebug() << "TabletCanvas::event default";
                if(ISDEFINE(lastpointzoom)){
                    /* si calcola la distanza tra il punto dell'ultimo touch e il punto corrente, e si usa il più lontano */
                    const double Dist1 = Distance(lastpointzoom[0].point, pointTouch);
                    const double Dist2 = Distance(this->lastpointzoom[1].point, pointTouch);

                    const bool FirstMoreDistance =  Dist1 > Dist2;
                    const QPointF &PointSelected = (FirstMoreDistance) ? lastpointzoom[0].point : lastpointzoom[1].point;

                    const double distanceSelected = Distance(PointSelected, pointTouch);

                    pointMiddle = puntoameta(lastpointzoom[0].point, pointTouch);

                    tmp_distance_right_left = Distance(lastpointzoom[0].point, lastpointzoom[1].point);

                    tmp_distance = distanceSelected / tmp_distance_right_left;

                    //qDebug() <<tmpNum<< "tmp_distance " << tmp_distance << "Distance_rigth_ledt" << tmp_distance_right_left << "Distance(PointSelected, pointTouch)" << distanceSelected;

                    needToResize = needToResize || this->zoom->zoom(pointMiddle, tmp_distance, widthPixmap, maxWidth, maxHeight, heightPixmap, data->datatouch);

                    //lastpointzoom[0].set = lastpointzoom[1].set = false;
                    lastpointzoom[FirstMoreDistance].point = pointTouch;

                    //qDebug() << tmpNum << "Index for update data" << int(FirstMoreDistance);
                }
                else{
                    const uchar index = !lastpointzoom[0].isNotDefine();
                    lastpointzoom[index].set = true;
                    lastpointzoom[index].point = pointTouch;
                    qDebug() << "Index: " << index;
                    continue;

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
            break;
            } // for touchPoints
        }

        update();
        break;
        }
        default:
            if(data != NULL)
                return QWidget::event(event);
    }
    //qDebug() << "After" << this->lastpointzoom[0].point << this->lastpointzoom[1].point << lastpointzoom[0].set << lastpointzoom[1].set << "\n";
    if(needToResize)
        this->callResizeEvent();

    if(data != NULL)
        return QWidget::event(event);

    return true;
}

static inline QPointF puntoameta(const QPointF &first, const QPointF &sec){
    static double x, y;
    x = (first.x() + sec.x()) / 2.0;
    y = (first.y() + sec.y()) / 2.0;

    return QPointF(x, y);
}

static inline double Distance(const QPointF &point1, const QPointF &point2){
    static double x, y;
    x = pow(point1.x() - point2.x(), 2);
    y = pow(point1.y() - point2.y(), 2);
    return sqrt(x + y);
}

static inline void scambio(QPointF &left, QPointF &right){
    double tmpval = left.x();
    if(left.x() > right.x()){
        left.setX(right.x());
        right.setX(tmpval);
    }

}
