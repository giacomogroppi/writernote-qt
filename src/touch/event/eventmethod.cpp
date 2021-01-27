#include "../tabletcanvas.h"
#include <QEvent>
#include <QDebug>
#include "math.h"
#include <QWheelEvent>

static bool isdefine(QPointF *point){
    return *point != QPointF(-1, -1);
}

static int calcolodistanza(QPointF *point1, QPointF *point2){
    return sqrt(pow(point1->x() - point2->x(), 2) + pow(point1->y() - point2->y(), 2));
}

bool TabletCanvas::event(QEvent *event){
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
                    continue;
                default:
                    {
                        QSizeF diams = touchPoint.ellipseDiameters();

                        /* a questo punto posso capire se è stato un zoom in o uno zoom on */
                        if(isdefine(&this->lastpointzoom.posd) && isdefine(&this->lastpointzoom.poss)){
                            QPointF punto = touchPoint.pos();
                            bool check = (calcolodistanza(&this->lastpointzoom.posd, &punto) < calcolodistanza(&this->lastpointzoom.poss, &punto));

                            if(!check){
                                if(punto.x() < this->lastpointzoom.poss.x() || punto.y() > this->lastpointzoom.poss.y()){
                                    posizionezoom_puntof.setX((punto.x() + lastpointzoom.posd.x())/2);
                                    posizionezoom_puntof.setY((punto.y() + lastpointzoom.posd.y())/2);

                                    this->zoomin();
                                }
                                else{
                                    posizionezoom_puntof.setX((punto.x() + lastpointzoom.posd.x())/2);
                                    posizionezoom_puntof.setY((punto.y() + lastpointzoom.posd.y())/2);

                                    this->zoomon();
                                }
                            }
                            else{
                                if(punto.x() > this->lastpointzoom.posd.x() || punto.y() < this->lastpointzoom.posd.y()){
                                    posizionezoom_puntof.setX((punto.x() + lastpointzoom.poss.x())/2);
                                    posizionezoom_puntof.setY((punto.y() + lastpointzoom.poss.y())/2);

                                    this->zoomin();
                                }
                                else{
                                    posizionezoom_puntof.setX((punto.x() + lastpointzoom.poss.x())/2);
                                    posizionezoom_puntof.setY((punto.y() + lastpointzoom.poss.y())/2);

                                    zoomon();
                                }
                            }

                            /* riazzera tutti e due i punti */
                            lastpointzoom.posd.setX(-1);
                            lastpointzoom.poss.setX(-1);

                            lastpointzoom.poss.setY(-1);
                            lastpointzoom.poss.setY(-1);

                            update();
                        }
                        /* altrimenti posso andare a salvare i punti iniziali */
                        else{
                            if(!isdefine(&this->lastpointzoom.posd))
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
