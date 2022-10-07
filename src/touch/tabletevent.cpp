#include "tabletcanvas.h"
#include <QDebug>
#include "utils/dialog_critic/dialog_critic.h"
#include "square/square.h"
#include "mainwindow.h"
#include "audioplay/audioplay.h"
#include "audiorecord/audiorecord.h"
#include "touch/laser/laser.h"
#include "utils/common_script.h"
#include "touch/object_finder/object_finder.h"
#include "preview_page_widget/preview_page_widget.h"
#include "touch/dataTouch/stroke/StrokePre.h"

#define MAXPOINT 20

StrokePre __tmp;
extern bool block_scrolling;

bool need_save_auto = false;
bool need_save_tmp = false;

static TabletPenMethod lastMethod;
static QEvent::Type eventType;

bool TabletCanvas::isWriting() const
{
    return !__tmp.isEmpty();
}

static void AppendAll(
        Document              &doc,
        const TabletCanvas    *canvas,
        const TabletPenMethod& met)
{
    /* for debug */
    StrokePre & strokeToAppend = __tmp;
    int i, pageMod, lenPoint;
    point_s *point;
    qint64 time;
    const QPointF &PointFirstPage = doc.getPointFirstPage();

    if(unlikely(strokeToAppend.isEmpty()))
        return;

    time = core::get_main_window()->m_audioplayer->getPositionSecond();

    /**
     * 2^31 ~ 2 Gs ~ 6.9 days. It's enough
     * */
    W_ASSERT(time < INT_MAX);

    strokeToAppend.adjust(PointFirstPage);


    if(unlikely(met.isLaser())){
        canvas->_laser->append(strokeToAppend);
        canvas->_laser->endMove();
        strokeToAppend = StrokePre();
    }else{
        const auto &stroke = strokeToAppend.merge();

        pageMod = doc.appendStroke(stroke);

        core::get_main_window()->_preview_widget->mod(pageMod);
        doc.at_mod(pageMod).triggerRenderImage(
                static_cast<int>(time),
                false
        );
    }

    strokeToAppend = StrokePre();
}

static inline bool is_rubber(QTabletEvent *event, const TabletPenMethod &met)
{
    W_ASSERT(event);
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
    const auto res = (event->pointerType() == QTabletEvent::PointerType::Eraser) or met.isRubber();
#else
    const auto res = (event->pointerType() == QPointingDevice::PointerType::Eraser) or met.isRubber();
#endif
    return res;
}

static not_used QString convert(const QEvent::Type e)
{
    switch(e){
        case QEvent::TabletPress:
            return "QEvent::TabletPress";
        case QEvent::TabletMove:
            return "QEvent::TabletMove";
        case QEvent::TabletRelease:
            return "QEvent::TabletRelease";
        default:
            std::abort();
    }
}

static force_inline bool is_out(const datastruct *data, const QPointF &point)
{
    const auto biggerX = data->biggerx();
    const auto biggerY = data->biggery();
    return  point.x() > biggerX or
            point.y() > biggerY or
            point.x() < 0.      or
            point.y() < 0.;
}

void TabletCanvas::tabletEvent(QTabletEvent *event)
{
    const QPointF& pointTouch = event->position();
    constexpr bool tabletDebug = false;

    _isWriting = true;
    need_save_auto = true;
    need_save_tmp = true;

    eventType = event->type();

    cbool isOut = is_out(getDoc(), pointTouch);

    WDebug(tabletDebug, event->type() << convert(event->type()));

    if(unlikely(isOut)){
        /*
         * the user is writing in a part where
         *  the sheet is not present.
         * You don't have to save the point. And
         * save the end of the current treatment
         */
        WDebug(tabletDebug, QString("Height: %1 Width: %2").arg(QString::number(height()), QString::number(width())) << pointTouch);
        ManageFinish(event, true);
        goto end;
    }

    if(unlikely(eventType == QEvent::TabletPress)){ /* first point */
        block_scrolling = true;
        ManageStart(event);
#if defined(WIN32) || defined(WIN64)
        this->isdrawing = true;
#endif
    }
    else if(likely(eventType == QEvent::TabletMove)){ /* user move the pen */
        ManageMove(event);
    }

#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
    else if(eventType == QEvent::TabletRelease){ /* pen leaves the tablet */
#else
    else if(eventType == QEvent::TabletRelease || eventType == QEvent::MouseButtonRelease){ /* pen leaves the tablet */
#endif
        WDebug(tabletDebug, "Tablet release");
        this->ManageFinish(event, false);
    }

end:

    if(unlikely(!_method.isSelection() && lastMethod.isSelection())){
        WDebug(tabletDebug, "Square reset");
        _square->reset();
    }

    update();

    event->accept();
    lastMethod = _method;
}

force_inline void ManageStartSquare(const QPointF &touch, class square *_square)
{
    constexpr auto not_used ManageStartdebugSquare = false;

    if(_square->somethingInBox()){
        WDebug(ManageStartdebugSquare, "Somethininbox");
        _square->initPointMove(touch);
    }
    else{
        WDebug(ManageStartdebugSquare, "not in box");
        _square->initPoint(touch);
    }
}

force_inline void TabletCanvas::ManageStart(QTabletEvent *event)
{
    constexpr const auto _debug = false;
    
    if(unlikely(m_deviceDown))
        return;

    W_ASSERT(event->type() == QEvent::TabletPress);

    if(_method.isInsert()){
        updatelist(event);
        _finder->move(event->position());
    }
    else if(_method.isSelection()){
        ManageStartSquare(event->position(), _square);
    }else if(is_rubber(event, _method)){
        _rubber->initRubber(event->position());
        WDebug(_debug, "rubber is set");
        W_ASSERT(_rubber->is_set());
    }

    m_deviceDown = true;
    _lastPoint.pos = event->position();
    _lastPoint.pressure = event->pressure();
}

force_inline void ManageMoveSquare(const QPointF &point, class square *_square)
{
    _square->isMoving();

    if(_square->somethingInBox()){
        W_ASSERT(_square->get_first_point().isSet());

        /* a questo punto può muovere di un delta x e y */
        _square->move(point);
    }else{
        /**
        * it means that the user not select anything
        * in the past
        */
        _square->updatePoint(point);
    }
}

force_inline void TabletCanvas::ManageMove(QTabletEvent *event)
{
    QPainter painter;
    constexpr not_used bool debugMove = false;
    const auto &point = event->position();

    if(event->deviceType() ==
#       if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
            QTabletEvent::RotationStylus
#       else
            QInputDevice::DeviceType::Stylus
#       endif
            ){
        updateCursor(event);
    }

    if(unlikely(!m_deviceDown))
        return;

    painter.begin(&_pixmap);
    W_ASSERT(painter.isActive());

    if(likely(_method.isInsert())){
        updateBrush(event);
    }

    _lastPoint.pos = event->position();
    _lastPoint.pressure = event->pressure();

    if(likely(_method.isInsert())){
        updatelist(event);
        _finder->move(point);
    }
    else if(is_rubber(event, _method)){
        _rubber->actionRubber(point);
    }
    else if(_method.isSelection()){
        ManageMoveSquare(point, _square);
    }else if(_method.isText()){
        if(_text_w->isIn(point)){
            
        }
        else{
            _text_w->createNew(point);
        }
    }

    W_ASSERT(painter.isActive());
    painter.end();
}

force_inline void TabletCanvas::ManageFinish(QTabletEvent *event, cbool isForce)
{
    int index_mod;
    bool done = _square->somethingInBox();
    block_scrolling = false;

#if defined(WIN32) || defined(WIN64)
    this->isdrawing = false;
#endif

    if(likely(_method.isInsert())){
        AppendAll(*this->getDoc(), this, _method);
        _finder->endMoving();
    }

    if(unlikely(!m_deviceDown)){
        if(_method.isSelection() && done){
            _square->reset();
        }
    }

    if (m_deviceDown && event->buttons() == Qt::NoButton){
        m_deviceDown = false;

        if(_method.isSelection()){
            if(!done){
                _square->find();
            }
            if(!isForce)
                _square->endMoving(this);
            else
                _square->hideProperty();

        }else if(is_rubber(event, _method)){
            index_mod = _rubber->endRubber();
            if(index_mod >= 0){
                core::get_main_window()->_preview_widget->mod(index_mod);
            }else if(index_mod != -1){
                core::get_main_window()->_preview_widget->mod(-1);
            }
        }
    }

    if(likely(_redoundo)){
        _redoundo->copy();
    }
}

void TabletCanvas::updatelist(QTabletEvent *event) const
{
    double size;
    uchar alfa;
    point_s tmp_point{};
    StrokePre &strokeTmp = __tmp;
    pressure_t pressure;
    cbool highlighter = is_rubber(event, _method);
    const QPointF &pointTouch = event->position();
    const auto *data = this->getDoc();
    const auto prop = data->getZoom() == PROP_RESOLUTION ?
                data->getZoom() :
                data->getZoom() - .0000001;

    size = event->pressure();
    alfa = unlikely(highlighter) ? _highlighter->getAlfa() : 255;

    if(unlikely(!this->m_deviceDown)){
        strokeTmp.setTime(static_cast<int>(
                                  core::get_main_window()->m_audio_recorder->getCurrentTime()
                ));
        strokeTmp.setColor(_color);
        strokeTmp.setAlfaColor(alfa);
    }

    tmp_point = pointTouch;
    pressure = unlikely(highlighter) ? _highlighter->getSize(size) : _pen_ui->getSize(size);

    strokeTmp.append(tmp_point, pressure, (QPen &)_pen, prop);
}
