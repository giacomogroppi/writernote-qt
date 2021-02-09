#include "../tabletcanvas.h"
#include <QEvent>
#include <QDebug>
#include "math.h"
#include <QWheelEvent>

static inline long double calcolodistanza(QPointF *point1, QPointF *point2);

static inline void scambio(QPointF *, QPointF *);

static inline void ridefine(struct lastpoint_struct *);
static inline QPointF puntoameta(QPointF &, QPointF &);

static inline bool its_inside(QPointF &left, QPointF &right, QPointF &move);

bool TabletCanvas::event(QEvent *event){
    QPointF temp;

    //qDebug() << event->type();

    switch (event->type()) {
        case QEvent::TouchBegin:
        case QEvent::TouchUpdate:
        case QEvent::TouchEnd:
        {
            const QTouchEvent *touch = static_cast<QTouchEvent *>(event);
            const QList<QTouchEvent::TouchPoint> touchPoints = static_cast<QTouchEvent *>(event)->touchPoints();
            for (const QTouchEvent::TouchPoint &touchPoint : touchPoints) {
                switch (touchPoint.state()) {
                case Qt::TouchPointStationary:
                case Qt::TouchPointReleased:
                    //ridefine(&lastpointzoom);
                    continue;
                default:
                    {
                        /* a questo punto posso capire se è stato un zoom in o uno zoom on */
                        if(this->lastpointzoom.posd != QPointF(-1, -1) &&
                                this->lastpointzoom.poss != QPointF(-1, -1)){
                            QPointF punto = touchPoint.pos();
                            bool check = (calcolodistanza(&this->lastpointzoom.posd, &punto) > calcolodistanza(&this->lastpointzoom.poss, &punto));

                            scambio(&lastpointzoom.poss, &lastpointzoom.posd);

                            /*
                             * if the registered point
                             * is closer to the left point
                            */
                            temp = puntoameta(lastpointzoom.poss, punto);

                            long double temp_distance;

                            if(check){
                                temp_distance = calcolodistanza(&lastpointzoom.posd, &punto)/calcolodistanza(&lastpointzoom.poss, &lastpointzoom.posd);
                                this->zoom->zoom(temp,
                                            temp_distance);

                            }
                            else{
                                temp_distance = calcolodistanza(&lastpointzoom.poss, &punto)/calcolodistanza(&lastpointzoom.posd, &lastpointzoom.poss);
                                this->zoom->zoom(temp,
                                                 temp_distance);
                            }

                            lastpointzoom.posd.setX(-1);
                            lastpointzoom.poss.setX(-1);

                            lastpointzoom.poss.setY(-1);
                            lastpointzoom.poss.setY(-1);

                            isloading = true;
                            update();
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
            break;
        }
        default:
            if(data != NULL)
                return QWidget::event(event);
    }
    return true;
}

static inline void ridefine(struct lastpoint_struct *point){
    point->posd = QPointF(-1, -1);
    point->poss = QPointF(-1, -1);
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
