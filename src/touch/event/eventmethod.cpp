#include "../tabletcanvas.h"
#include <QEvent>
#include "math.h"
#include <QWheelEvent>
#include <QDebug>

static inline double calcolodistanza(const QPointF &point1, const QPointF &point2);
static inline void scambio(QPointF &, QPointF &);
static inline QPointF puntoameta(QPointF &, QPointF &);

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

    static bool check;
    static QPointF punto, tmp;
    static double tmp_distance;
    static double tmp_distance_right_left;

    const int height = this->height();
    const int width = this->width();

    const int heightPixmap = this->m_pixmap.height();
    const int widthPixmap = this->m_pixmap.width();

    const auto type = event->type();
    //qDebug() << type;

    if(!data)
        return QWidget::event(event);

    if(type != QEvent::TouchEnd)
        goto stop;
    switch (type) {
        case QEvent::TouchBegin:
        case QEvent::TouchUpdate:
        case QEvent::TouchEnd:
        {
            const QList<QTouchEvent::TouchPoint> touchPoints = static_cast<QTouchEvent *>(event)->touchPoints();
            for (const QTouchEvent::TouchPoint &touchPoint : touchPoints) {
                switch (touchPoint.state()) {
                case Qt::TouchPointStationary:
                case Qt::TouchPointReleased:
                    continue;
                default:
                    {
                        if(ISDEFINE(lastpointzoom)){
                            punto = touchPoint.pos();
                            check = (calcolodistanza(this->lastpointzoom[1].point, punto) > calcolodistanza(this->lastpointzoom[0].point, punto));

                            scambio(lastpointzoom[0].point, lastpointzoom[1].point);

                            /*
                             * if the registered point
                             * is closer to the left point
                            */
                            pointMiddle = puntoameta(lastpointzoom[0].point, punto);

                            tmp_distance_right_left = calcolodistanza(lastpointzoom[0].point, lastpointzoom[1].point);

                            if(check){
                                tmp_distance = calcolodistanza(lastpointzoom[1].point, punto)/tmp_distance_right_left;
                                needToResize = needToResize || this->zoom->zoom(pointMiddle,
                                            tmp_distance,
                                            widthPixmap,
                                            width,
                                            heightPixmap,
                                            height,
                                            data->datatouch);

                            }
                            else{
                                tmp_distance = calcolodistanza(lastpointzoom[0].point, punto)/tmp_distance_right_left;
                                needToResize = needToResize || this->zoom->zoom(pointMiddle,
                                            tmp_distance,
                                            widthPixmap,
                                            width,
                                            heightPixmap,
                                            height,
                                            data->datatouch);
                            }

                            RIDEFINE(lastpointzoom);

                        }
                        else{
                            if(!this->lastpointzoom[1].set){
                                this->lastpointzoom[1].point = touchPoint.pos();
                            }
                            else{
                                if(touchPoint.pos().x() > this->lastpointzoom[1].point.x()){
                                    /* deve fare lo scambio */
                                    tmp = this->lastpointzoom[1].point;
                                    this->lastpointzoom[1].point = touchPoint.pos();
                                    this->lastpointzoom[0].point = tmp;
                                }
                                else
                                    /* altrimenti vuol dire che il punto a destra è già quello a destra */
                                    this->lastpointzoom[0].point= touchPoint.pos();
                            }
                        }

                    }
                    break;
                }
            }

            update();
            break;
        }
        default:
            if(data != NULL)
                return QWidget::event(event);
    }
    if(needToResize)
        this->resizeEvent(NULL);
    stop:
    if(data != NULL)
        return QWidget::event(event);

    return true;
}

static inline QPointF puntoameta(QPointF &puntouno, QPointF &puntodue){
    return QPointF((puntouno.x() + puntodue.x())/2,
                (puntouno.y() + puntodue.y())/2);
}

static inline double calcolodistanza(const QPointF &point1, const QPointF &point2){
    return sqrt(pow(point1.x() - point2.x(), 2) + pow(point1.y() - point2.y(), 2));
}

static inline void scambio(QPointF &left, QPointF &right){
    double tmpval = left.x();
    if(left.x() > right.x()){
        left.setX(right.x());
        right.setX(tmpval);

        tmpval = left.y();
        left.setY(right.y());
        right.setY(tmpval);
    }

}
