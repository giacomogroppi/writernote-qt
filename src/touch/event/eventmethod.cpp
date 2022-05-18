#include "core/core.h"
#include "touch/tabletcanvas.h"
#include <QEvent>
#include "math.h"
#include <QWheelEvent>
#include <QDebug>
#include "mainwindow.h"

static force_inline QPointF PointMiddle(const QPointF &first, const QPointF &sec)
{
    double x, y;
    x = (first.x() + sec.x()) / 2.0;
    y = (first.y() + sec.y()) / 2.0;

    return QPointF(x, y);
}

extern bool isZooming;

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

static int get_index(TabletCanvas *canvas, const QPointF &pointTouch)
{
    PointSettable *last = canvas->lastpointzoom;
    const double Dist1 = distance_not_square(last[0].point, pointTouch);
    const double Dist2 = distance_not_square(last[1].point, pointTouch);
    return Dist1 > Dist2;
}

class WEvent{
private:
    const QEvent *_event;
    enum WEventEnum{
        BEGIN_TOUCH = 0,
        STOP_TOUCH = 1,
        UPDATE_TOUCH = 2,
        NOT_TOUCH = 3,
        NOT_CONSIDER = 4
    } _e;

    force_inline void setPrivateEvent()
    {
#if QT_VERSION > QT_VERSION_CHECK(6, 0, 0)
        QEvent::Type type;
        static bool is_touch = false;
        static bool is_touch_second = false;

       // qDebug() << event->type();

        switch (_event->type()) {
        case QEvent::MouseMove:
            _e = (is_touch) ? WEvent::UPDATE_TOUCH : WEvent::NOT_TOUCH;
            break;
        case QEvent::TouchBegin:
            is_touch = true;
            is_touch_second = true;
            _e = WEvent::BEGIN_TOUCH;
            break;
        case QEvent::MouseButtonPress:
            _e = (is_touch_second) ? NOT_CONSIDER : WEvent::NOT_TOUCH;
            is_touch_second = false;
            break;
        default:
            _e = NOT_TOUCH;
        }
#else
        switch (_event->type()) {
        case QEvent::TouchBegin:
            _e = WEvent::BEGIN_TOUCH;
            break;
        case QEvent::TouchUpdate:
            _e = WEvent::UPDATE_TOUCH;
            break;
        case QEvent::TouchEnd:
            _e = WEvent::STOP_TOUCH;
            break;
        default:
            _e = WEvent::NOT_TOUCH;
        }
#endif
    }

public:
    force_inline WEvent(const QEvent *event)
    {
        _event = event;
        this->setPrivateEvent();
    }
    
    force_inline bool is_touch() const
    {
        return this->_e != NOT_TOUCH;
    }

    force_inline bool is_begin() const
    {
        W_ASSERT(this->is_touch());
        return this->_e == BEGIN_TOUCH;
    }

    force_inline bool is_end() const
    {
        W_ASSERT(this->is_touch());
        return this->_e == STOP_TOUCH;
    }

    force_inline bool is_update() const
    {
        W_ASSERT(this->is_touch());
        return this->_e == UPDATE_TOUCH;
    }

    force_inline bool not_consider() const
    {
        W_ASSERT(this->is_touch());
        return this->_e == NOT_CONSIDER;
    }

    auto get_points() const
    {
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
    const QList<QTouchEvent::TouchPoint> touchPoints = ((QTouchEvent *)_event)->touchPoints();
#else
    const auto touchPoints = ((QMouseEvent *)_event)->points();
#endif
    return touchPoints;
    }
};

bool TabletCanvas::event(QEvent *event)
{
    constexpr bool not_used TabletEventDebug = true;
    constexpr auto not_used name = "TabletCanvas::event";

    bool needToResize = false;
    QPointF _pointMiddle;

    const QSize maxSize = this->size();
    const QSize size = this->_pixmap.size();

    bool zoomChange = false;

    const auto WE = WEvent(event);    

    //WDebug(true, name << type);

    if(!WE.is_touch() || unlikely(WE.not_consider())){
        return QWidget::event(event);
    }

    W_ASSERT(WE.is_touch());

    if(WE.is_end()){
ridefine:
        WDebug(TabletEventDebug, name << "Ridefine");
        RIDEFINE(this->lastpointzoom);
        block_scrolling = false;
        isZooming = false;
        return QWidget::event(event);
    }

    const auto touchPoints = WE.get_points();

    // se l'utente sta zoomando con le dita touchPoints ha per forza lunghezza due.
    if(unknown(touchPoints.length() != 2)){
        return QWidget::event(event);
    }

    bool somethingCtrl = false;
    QPointF point[2];
    int index_other[2] = { -1, -1};

    block_scrolling = true;

    for(int i = 0; i < 2; i++)
    {
        const auto &touchPoint = touchPoints.at(i);
        const QPointF &pointTouch = touchPoint.pos();
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
        cbool _tmp =    touchPoint.state() == Qt::TouchPointPressed ||
                        touchPoint.state() == Qt::TouchPointMoved ||
                        touchPoint.state() == Qt::TouchPointStationary;
#else
        cbool _tmp = touchPoint.state() == QEventPoint::Pressed ||
                     touchPoint.state() == QEventPoint::Updated ||
                     touchPoint.state() == QEventPoint::Stationary;
#endif
        WDebug(TabletEventDebug, name << _tmp << touchPoint.state());

        if(!_tmp){
            continue;
        }

        if(!ISDEFINE(lastpointzoom)){
            setPoint(pointTouch, this);
            continue;
        }

        if(distance(lastpointzoom[0].point, lastpointzoom[1].point) < 200.)
        {
            goto ridefine;
        }

        somethingCtrl = true;

        /* si calcola la distanza tra il punto dell'ultimo touch e il punto corrente, e si usa il più lontano */
        cint IndexSave = get_index(this, pointTouch);

        // First cicle
        if(!i){
            point[IndexSave] = pointTouch;
            index_other[i] = IndexSave;
            continue;
        }

        // Second cicle
        index_other[i] = IndexSave;

        // se abbiamo salvato prima in 0 adesso dobbiamo salvare in 1
        int tmp = index_other[0] == 0 ? 1 : 0;
        point[tmp] = pointTouch;
    }

    if(!somethingCtrl)
        goto out;

    if((index_other[0] < 0) || (index_other[1] < 0)){
        goto out;
    }

    if(index_other[0] == index_other[1])
        goto out;

    //qDebug() << "index 0: " << index_other[0] << index_other[1]
    //         << "point 0: " << point[0] << "point 1: " << point[1];

    {
        // current distance
        const double distanceSelected         = distance(point[0],                point[1]);

        // original distance
        const double tmp_distance_right_left  = distance(lastpointzoom[0].point,  lastpointzoom[1].point);

        const double multiplier = distanceSelected / tmp_distance_right_left;

        _pointMiddle = PointMiddle(point[0], point[1]);
        needToResize = _zoom->zoom(_pointMiddle, multiplier, zoomChange, size, maxSize, data->datatouch);
    }

    lastpointzoom[0].point = point[0];
    lastpointzoom[1].point = point[1];

    //qDebug() << "After" << this->lastpointzoom[0].point << this->lastpointzoom[1].point << lastpointzoom[0].set << lastpointzoom[1].set << "\n";
    if(needToResize)
        this->callResizeEvent();

    if(zoomChange)
        core::get_main_window()->zoomChange();

out:
    update();
    return QWidget::event(event);
}
