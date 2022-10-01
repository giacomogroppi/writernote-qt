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

#define MAXPOINT 20

Stroke __tmp;
extern bool block_scrolling;

bool need_save_auto = false;
bool need_save_tmp = false;

static TabletPenMethod lastMethod;
static QEvent::Type eventType;

static void AppendAll(
        Document              &doc,
        const TabletCanvas    *canvas,
        const TabletPenMethod& met)
{
    /* for debug */
    Stroke & strokeToAppend = __tmp;
    int i, pageMod, lenPoint;
    point_s *point;
    qint64 time;
    const QPointF &PointFirstPage = doc.datatouch->getPointFirstPage();

    if(unlikely(strokeToAppend.isEmpty()))
        return;

    time = core::get_main_window()->m_audioplayer->getPositionSecond();

    W_ASSERT(time < INT_MAX);

    if(likely(strokeToAppend.is_normal())){
        lenPoint = strokeToAppend.length();
        for(i = 0; i < lenPoint; i++){
            point = &strokeToAppend.at_mod(i);
            point->_x -= PointFirstPage.x();
            point->_y -= PointFirstPage.y();
        }
    }else{
        stroke_complex_translate(&strokeToAppend, -PointFirstPage);
    }

    if(unlikely(met.isLaser())){
        canvas->_laser->append(strokeToAppend);
        canvas->_laser->endMove();
    }else{
        pageMod = doc.datatouch->appendStroke(strokeToAppend);

        core::get_main_window()->_preview_widget->mod(pageMod);
        doc.datatouch->at_mod(pageMod).triggerRenderImage(
                static_cast<int>(time),
                false
        );
    }

    strokeToAppend.reset();
}

static not_used QString convert(const QEvent::Type eventType)
{
    switch(eventType){
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

static force_inline void set_flag(const QTabletEvent *event, TabletPenMethod met)
{
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
    rubber_method |= (event->pointerType() == QTabletEvent::PointerType::Eraser);
#else
    rubber_method |= (event->pointerType() == QPointingDevice::PointerType::Eraser);
#endif

    if(unlikely(rubber_method)){
        setFalse();
    }
    else{
        CTRL_METHOD(insert_method, highlighter);
        CTRL_METHOD_OR(insert_method, pen);
        CTRL_METHOD_OR(insert_method, laser);
        CTRL_METHOD(selection_method, selection);
        CTRL_METHOD(text_method, text);
    }
}

static force_inline bool is_out(const datastruct *data, const QPointF &point)
{
    return point.x() > data->biggerx() or point.y() > data->biggery() or
            point.x() < 0. or point.y() < 0.;
}

void TabletCanvas::tabletEvent(QTabletEvent *event)
{
    const QPointF& pointTouch = event->position();
    constexpr bool tabletDebug = false;

    isWriting = true;
    need_save_auto = true;
    need_save_tmp = true;

    eventType = event->type();

    set_flag(event, _input);

    cbool isOut = is_out(data->datatouch, pointTouch);

    WDebug(tabletDebug, event->type() << convert(event->type()) << convert_method());

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

    if(unlikely(!selection_method && lastMethod == e_method::selection)){
        WDebug(tabletDebug, "Square reset");
        _square->reset();
    }

    update();

    event->accept();
    lastMethod = _input;
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

    if(insert_method){
        updatelist(event);
        _finder->move(event->position());
    }
    else if(selection_method){
        ManageStartSquare(event->position(), _square);
    }else if(rubber_method){
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

    if(likely(insert_method)){
        updateBrush(event);
    }

    _lastPoint.pos = event->position();
    _lastPoint.pressure = event->pressure();

    if(likely(insert_method)){
        updatelist(event);
        _finder->move(point);
    }
    else if(rubber_method){
        _rubber->actionRubber(point);
    }
    else if(selection_method){
        ManageMoveSquare(point, _square);
    }else if(text_method){
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

    if(likely(insert_method)){
        AppendAll(*this->data, this, _input);
        _finder->endMoving();
    }

    if(unlikely(!m_deviceDown)){
        if(selection_method && done){
            _square->reset();
        }
    }

    if (m_deviceDown && event->buttons() == Qt::NoButton){
        m_deviceDown = false;

        if(selection_method){
            if(!done){
                _square->find();
            }
            if(!isForce)
                _square->endMoving(this);
            else
                _square->hideProperty();

        }else if(rubber_method){
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
    point_s tmp_point;
    Stroke &strokeTmp = __tmp;
    pressure_t pressure;
    cbool highlighter = CHECK_FLAG(_input, highlighter);
    const QPointF &pointTouch = event->position();

    size = event->pressure();
    alfa = unlikely(highlighter) ? _highlighter->getAlfa() : 255;

    if(unlikely(!this->m_deviceDown)){
        strokeTmp.setPositioneAudio((int) core::get_main_window()->m_audio_recorder->getCurrentTime());
        strokeTmp.setColor(_color);
        strokeTmp.setAlfaColor(alfa);
    }

    tmp_point._x = pointTouch.x();
    tmp_point._y = pointTouch.y();
    pressure = unlikely(highlighter) ? _highlighter->getSize(size) : _pen_ui->getSize(size);

    strokeTmp.append(tmp_point, pressure);
}
