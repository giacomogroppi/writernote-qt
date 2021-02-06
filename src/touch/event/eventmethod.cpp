#include "../tabletcanvas.h"
#include <QEvent>
#include <QDebug>
#include "math.h"
#include <QWheelEvent>

static inline long double calcolodistanza(QPointF *point1, QPointF *point2);

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
                    ridefine(&lastpointzoom);
                    continue;
                default:
                    {
                        /* a questo punto posso capire se è stato un zoom in o uno zoom on */
                        if(this->lastpointzoom.posd != QPointF(-1, -1) &&
                                this->lastpointzoom.poss != QPointF(-1, -1)){
                            QPointF punto = touchPoint.pos();
                            bool check = (calcolodistanza(&this->lastpointzoom.posd, &punto) > calcolodistanza(&this->lastpointzoom.poss, &punto)), d;

                            //d = its_inside(lastpointzoom.poss, lastpointzoom.posd, punto);

                            /*
                             * if the registered point
                             * is closer to the left point
                            */
                            temp = puntoameta(lastpointzoom.poss, punto);
                            if(check){
                                this->zoom->zoom(temp,
                                            calcolodistanza(&lastpointzoom.poss, &punto)/calcolodistanza(&lastpointzoom.poss, &lastpointzoom.posd));


                                /*if(punto.x() < this->lastpointzoom.poss.x() || punto.y() > this->lastpointzoom.poss.y()){
                                    posizionezoom_puntof.setX((punto.x() + lastpointzoom.posd.x())/2);
                                    posizionezoom_puntof.setY((punto.y() + lastpointzoom.posd.y())/2);

                                    zoomin();
                                }
                                else{
                                    posizionezoom_puntof.setX((punto.x() + lastpointzoom.posd.x())/2);
                                    posizionezoom_puntof.setY((punto.y() + lastpointzoom.posd.y())/2);

                                    zoomon();
                                }*/
                            }
                            else{
                                this->zoom->zoom(temp,
                                                 calcolodistanza(&lastpointzoom.poss, &lastpointzoom.poss)/calcolodistanza(&lastpointzoom.posd, &punto));
                                /*if(punto.x() > this->lastpointzoom.posd.x() || punto.y() < this->lastpointzoom.posd.y()){
                                    posizionezoom_puntof.setX((punto.x() + lastpointzoom.poss.x())/2);
                                    posizionezoom_puntof.setY((punto.y() + lastpointzoom.poss.y())/2);

                                    zoomin();
                                }
                                else{
                                    posizionezoom_puntof.setX((punto.x() + lastpointzoom.poss.x())/2);
                                    posizionezoom_puntof.setY((punto.y() + lastpointzoom.poss.y())/2);

                                    zoomon();
                                }*/
                            }

                            lastpointzoom.posd.setX(-1);
                            lastpointzoom.poss.setX(-1);

                            lastpointzoom.poss.setY(-1);
                            lastpointzoom.poss.setY(-1);

                            update();
                        }
                        /* altrimenti posso andare a salvare i punti iniziali */
                        else{
                            if(this->lastpointzoom.posd != QPointF(-1, -1))
                                this->lastpointzoom.posd = touchPoint.pos();

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
    return QPointF((puntouno.x() - puntodue.x())/2,
                (puntouno.y() + puntodue.y())/2);
}

static inline long double calcolodistanza(QPointF *point1, QPointF *point2){
    return sqrt(pow(point1->x() - point2->x(), 2) + pow(point1->y() - point2->y(), 2));
}

/*static inline bool its_inside(QPointF &left, QPointF &right, QPointF &move){
    bool check;
    check = left.x() <= move.x() && right.x() >= move.x();
    return check;
}*/
