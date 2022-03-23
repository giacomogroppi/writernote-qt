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
#define ISDEFINE(x) ((x[1].set)&&(x[0].set))
#define RIDEFINE(x) x[0].set = x[1].set = false;

bool block_scrolling = false;

static void setPoint(const QPointF &pointTouch, TabletCanvas *canvas)
{
    PointSettable *last = canvas->lastpointzoom;
    const uchar index = last[0].set;
    last[index].set = true;
    last[index].point = pointTouch;
}

bool TabletCanvas::event(QEvent *event)
{
    constexpr bool TabletEventDebug = true;

    bool needToResize = false;
    QPointF _pointMiddle;

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

        bool somethingCtrl = false;
        QPointF point[2];
        char which = -1;

        // 0 first point that is double
        // 1 second point that is double
        constexpr char split = 2; // 2 1 or 2

        block_scrolling = true;

        for(int i = 0; i < 2; i++)
        {
            const auto &touchPoint = touchPoints.at(i);
            const QPointF &pointTouch = touchPoint.pos();
            cbool _tmp =    touchPoint.state() == Qt::TouchPointPressed ||
                            touchPoint.state() == Qt::TouchPointMoved ||
                            touchPoint.state() == Qt::TouchPointStationary;
            if(!_tmp)
                continue;

            somethingCtrl = true;

            if(!ISDEFINE(lastpointzoom)){
                setPoint(pointTouch, this);
                continue;
            }

            /* si calcola la distanza tra il punto dell'ultimo touch e il punto corrente, e si usa il più lontano */
            const auto Dist1 = Distance(lastpointzoom[0].point, pointTouch);
            const auto Dist2 = Distance(lastpointzoom[1].point, pointTouch);
            const int FirstMoreDistance =  Dist1 > Dist2;
            
            if(!i){
                point[FirstMoreDistance] = pointTouch;
                which = FirstMoreDistance;
                continue;
            }

            int newIndex = (which == 0) ? 1 : 0;

            // un dito è fermo (o il primo o il secondo)
            if(FirstMoreDistance == which){
                // bisogna settare il punto sull quelli che abbiamo salvato
                // precedentemente
                point[newIndex] = lastpointzoom[newIndex].point;


                // non modifichiamo which in quanto segna ancora il primo punto
                goto do_zoom;
            }

            point[newIndex] = pointTouch;

            which = split;
        }

        do_zoom:

        const auto mes = qstr("Which: %1")  .arg(QString::number(which));
        qDebug() << mes << "0: " << point[0] << "1: " << point[1];

        return QWidget::event(event);

        {
            // current distance
            const double distanceSelected         = Distance(point[0],                point[1]);

            // original distance
            const double tmp_distance_right_left  = Distance(lastpointzoom[0].point,  lastpointzoom[1].point);

            const double multiplier = distanceSelected / tmp_distance_right_left;

            _pointMiddle = PointMiddle(point[0], point[1]);
            needToResize = needToResize || _zoom->zoom(_pointMiddle, multiplier, zoomChange, size, maxSize, data->datatouch);
        }

        if(somethingCtrl){
            lastpointzoom[0].point = point[0];
            lastpointzoom[1].point = point[1];
        }

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
