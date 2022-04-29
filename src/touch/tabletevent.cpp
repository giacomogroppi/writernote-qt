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
#define CHECK_FLAG(var, type) var == TabletCanvas::e_method::type
#define CTRL_METHOD(var, type) var = met == TabletCanvas::e_method::type
#define CTRL_METHOD_OR(var, type) var |= met == TabletCanvas::e_method::type

stroke __tmp;
extern bool block_scrolling;

bool need_save_auto = false;
bool need_save_tmp = false;

static bool insert_method;
static bool selection_method;
static bool rubber_method;
static bool text_method;

static TabletCanvas::e_method lastMethod = TabletCanvas::e_method::pen;
static QEvent::Type eventType;

static void AppendAll(
        Document                        &doc,
        const TabletCanvas              *canvas,
        const TabletCanvas::e_method    met)
{
    /* for debug */
    stroke & strokeToAppend = __tmp;
    int i, pageMod, lenPoint;
    point_s *point;
    int time;
    const QPointF &PointFirstPage = doc.datatouch->getPointFirstPage();

    if(unlikely(strokeToAppend.isEmpty()))
        return;

    time = canvas->_parent->m_audioplayer->getPositionSecond();

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

    if(unlikely(met == TabletCanvas::e_method::laser)){
        canvas->_laser->append(strokeToAppend);
        canvas->_laser->endMove();
    }else{
        pageMod = doc.datatouch->appendStroke(strokeToAppend);

        canvas->_parent->_preview_widget->mod(pageMod);
        doc.datatouch->at_mod(pageMod).triggerRenderImage(time, false);
    }

    strokeToAppend.reset();
}

static not_used QString convert_method()
{
    QString method;
    int index;

    if(rubber_method)
        method += "rubber ";
    if(insert_method)
        method += "pen highlighter laser ";
    if(selection_method)
        method += "square ";
    if(text_method)
        method += "text ";

    if(method.isEmpty())
        std::abort();

    index = method.lastIndexOf(' ');
    method = method.left(index);


    return method;
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

static force_inline void setFalse()
{
    insert_method = false;
    selection_method = false;
    text_method = false;
}

static force_inline void set_flag(const QTabletEvent *event, TabletCanvas::e_method met)
{
    CTRL_METHOD(rubber_method, rubber);
    rubber_method |= (event->pointerType() == QTabletEvent::PointerType::Eraser);

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

void TabletCanvas::tabletEvent(QTabletEvent *event)
{
    const QPointF& pointTouch = event->posF();
    constexpr bool tabletDebug = false;
    constexpr const char *nameFunction = "TabletCanvas::tabletEvent";

    isWriting = true;
    need_save_auto = true;
    need_save_tmp = true;

    eventType = event->type();

    set_flag(event, _input);

    WDebug(tabletDebug, nameFunction << event->type() << convert(event->type()) << convert_method());

    if(unlikely(    pointTouch.x() > data->datatouch->biggerx()
                ||  pointTouch.y() > data->datatouch->biggery())){
        /* the user is writing in a part where
         *  the sheet is not present.
         * You don't have to save the point. And
         * save the end of the current treatment
         */
        WDebug(tabletDebug, nameFunction << QString("Height: %1 Width: %2").arg(QString::number(height()), QString::number(width())) << pointTouch);
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

    else if(eventType == QEvent::TabletRelease){ /* pen leaves the tablet */
        this->ManageFinish(event, false);
    }

end:

    if(unlikely(!selection_method && lastMethod == e_method::selection)){
        WDebug(tabletDebug, nameFunction << "Square reset");
        _square->reset();
    }

    update();

    event->accept();
    lastMethod = _input;
}


force_inline void ManageStartSquare(const QPointF &touch, class square *_square)
{
    constexpr const auto not_used debugSquare = false;
    if(_square->somethingInBox()){
        WDebug(debugSquare, "TabletCanvas::ManageStart" << "Somethininbox");
        _square->initPointMove(touch);
    }
    else{
        WDebug(debugSquare, "TabletCanvas::ManageStart" << "not in box");
        _square->initPoint(touch);
    }
}

force_inline void TabletCanvas::ManageStart(QTabletEvent *event)
{
    if(unlikely(m_deviceDown))
        return;

    if(insert_method){
        updatelist(event);
        _finder->move(event->posF());
    }
    else if(selection_method){
        ManageStartSquare(event->posF(), _square);
    }else if(rubber_method){
        _rubber->initRubber(event->posF());
    }

    m_deviceDown = true;
    _lastPoint.pos = event->pos();
    _lastPoint.pressure = event->pressure();
}

force_inline void ManageMoveSquare(const QPointF &point, class square *_square)
{
    _square->isMoving();

    if(_square->somethingInBox()){
        W_ASSERT(!_square->get_first_point().isNotSet());

        /* a questo punto può muovere di un delta x e y */
        _square->move(point);
    }else{
        /*
        * it means that the user not select anything
        * in the past
        */
        _square->updatePoint(point);
    }
}

force_inline void TabletCanvas::ManageMove(QTabletEvent *event)
{
    QPainter painter;
    constexpr not_used bool debugMove = true;
    const auto &point = event->posF();

    if(event->deviceType() == QTabletEvent::RotationStylus){
        updateCursor(event);
    }

    if(unlikely(!m_deviceDown))
        return;

    painter.begin(&_pixmap);
    W_ASSERT(painter.isActive());

    if(likely(insert_method)){
        updateBrush(event);
    }

    _lastPoint.pos = event->pos();
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

        }else if(rubber_method){
            index_mod = _rubber->endRubber();
            if(index_mod >= 0){
                this->_parent->_preview_widget->mod(index_mod);
            }else if(index_mod != -1){
                this->_parent->_preview_widget->mod(-1);
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
    stroke &strokeTmp = __tmp;
    pressure_t pressure;
    cbool highlighter = CHECK_FLAG(_input, highlighter);
    const QPointF &pointTouch = event->posF();

    size = event->pressure();
    alfa = unlikely(highlighter) ? _highlighter->getAlfa() : 255;

    if(unlikely(!this->m_deviceDown)){
        strokeTmp.setPositioneAudio(_parent->m_audio_recorder->getCurrentTime());
        strokeTmp.setColor(_color);
        strokeTmp.setAlfaColor(alfa);
    }

    tmp_point._x = pointTouch.x();
    tmp_point._y = pointTouch.y();
    pressure = unlikely(highlighter) ? _highlighter->getSize(size) : _pen_ui->getSize(size);

    strokeTmp.append(tmp_point, pressure);
}
