#include "../tabletcanvas.h"
#include <QEvent>
#include "math.h"
#include <QWheelEvent>
#include <QDebug>

static inline long double calcolodistanza(QPointF *point1, QPointF *point2);

static inline void scambio(QPointF *, QPointF *);

static inline QPointF puntoameta(QPointF &, QPointF &);

#define ISDEFINE_RIGHT(x) x.posd!=QPointF(-1,-1)
#define ISDEFINE_LEFT(x) x.poss!=QPointF(-1,-1)

#define ISDEFINE(x) ISDEFINE_RIGHT(x)&&ISDEFINE_LEFT(x)

#define PRIVATE_RIDEFINE -1,-1
#define RIDEFINE_POINT(x) x=QPointF(PRIVATE_RIDEFINE)
#define RIDEFINE(x) RIDEFINE_POINT(x.poss),RIDEFINE_POINT(x.posd)

/*
 * this function only manage zoom
*/

static QPointF temp;
static bool needToResize;

bool TabletCanvas::event(QEvent *event){
    needToResize = false;

    auto type = event->type();

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
                            QPointF punto = touchPoint.pos();
                            bool check = (calcolodistanza(&this->lastpointzoom.posd, &punto) > calcolodistanza(&this->lastpointzoom.poss, &punto));

                            scambio(&lastpointzoom.poss, &lastpointzoom.posd);

                            /*
                             * if the registered point
                             * is closer to the left point
                            */
                            temp = puntoameta(lastpointzoom.poss, punto);

                            long double temp_distance;
                            long double temp_distance_right_left = calcolodistanza(&lastpointzoom.poss, &lastpointzoom.posd);

                            if(check){
                                temp_distance = calcolodistanza(&lastpointzoom.posd, &punto)/temp_distance_right_left;
                                needToResize = needToResize || this->zoom->zoom(temp,
                                            temp_distance,
                                            m_pixmap.width(),
                                            this->width(),
                                            m_pixmap.height(),
                                            this->height(),
                                            data->datatouch);

                            }
                            else{
                                temp_distance = calcolodistanza(&lastpointzoom.poss, &punto)/temp_distance_right_left;
                                needToResize = needToResize || this->zoom->zoom(temp,
                                            temp_distance,
                                            m_pixmap.width(),
                                            this->width(),
                                            m_pixmap.height(),
                                            this->height(),
                                            data->datatouch);
                            }

                            RIDEFINE(lastpointzoom);

                        }
                        else{
                            if(this->lastpointzoom.posd == QPointF(-1, -1)){
                                this->lastpointzoom.posd = touchPoint.pos();
                            }
                            else{
                                if(touchPoint.pos().x() > this->lastpointzoom.posd.x()){
                                    /* deve fare lo scambio */
                                    QPointF temp = this->lastpointzoom.posd;
                                    this->lastpointzoom.posd = touchPoint.pos();
                                    this->lastpointzoom.poss = temp;
                                }
                                else
                                    /* altrimenti vuol dire che il punto a destra è già quello a destra */
                                    this->lastpointzoom.poss = touchPoint.pos();
                            }
                        }

                    }
                    break;
                }
            }
            if(needToResize)
                this->resizeEvent(NULL);

            update();
            break;
        }
        default:
            if(data != NULL)
                return QWidget::event(event);
    }
    if(data != NULL)
        return QWidget::event(event);
    //return true;
    return true;
}

static inline QPointF puntoameta(QPointF &puntouno, QPointF &puntodue){
    return QPointF((puntouno.x() + puntodue.x())/2,
                (puntouno.y() + puntodue.y())/2);
}

static inline long double calcolodistanza(QPointF *point1, QPointF *point2){
    return sqrt(pow(point1->x() - point2->x(), 2) + pow(point1->y() - point2->y(), 2));
}

static inline void scambio(QPointF *left, QPointF *right){
    if(left->x() > right->x()){
        long double temp = left->x();
        left->setX(right->x());
        right->setX(temp);

        temp = left->y();
        left->setY(right->y());
        right->setY(temp);
    }

}
