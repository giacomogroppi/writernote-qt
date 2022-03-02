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
    uint i;

    /* for debug */
    stroke & strokeToAppend = __tmp;
    int pageMod;
    const uint lenPoint = strokeToAppend.length();
    point_s *point;
    const QPointF &PointFirstPage = doc.datatouch->getPointFirstPage();

    if(unlikely(!lenPoint))
        return;

    int time = canvas->parent->m_audioplayer->getPositionSecond();

    for(i = 0; i < lenPoint; i++){
        point = &strokeToAppend.at_mod(i);
        point->_x -= PointFirstPage.x();
        point->_y -= PointFirstPage.y();
    }

    if(unlikely(met == TabletCanvas::e_method::laser)){
        canvas->m_laser->append(strokeToAppend);
        canvas->m_laser->endMove();
    }else{
        pageMod = doc.datatouch->appendStroke(strokeToAppend);

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

    set_flag(event, this->medotodiinserimento);

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
        ManageStart(event, pointTouch);
#if defined(WIN32) || defined(WIN64)
        this->isdrawing = true;
#endif
    }
    else if(likely(eventType == QEvent::TabletMove)){ /* user move the pen */
        ManageMove(event, pointTouch);
    }

    else if(eventType == QEvent::TabletRelease){ /* pen leaves the tablet */
        this->ManageFinish(event, false);
    }

end:

    if(unlikely(!selection_method && lastMethod == e_method::selection)){
        WDebug(tabletDebug, nameFunction << "Square reset");
        m_square->reset();
    }

    update();

    event->accept();
    lastMethod = this->medotodiinserimento;
}

force_inline void TabletCanvas::ManageStart(
        QTabletEvent    *event,
        const QPointF   &pointTouch)
{
    constexpr const auto debugSquare = true;

    if(unlikely(m_deviceDown))
        return;

    if(insert_method){
        updatelist(event);
        m_finder->move();
    }
    else if(selection_method){
        if(m_square->somethingInBox()){
            WDebug(debugSquare, "TabletCanvas" << __FUNCTION__ << "Somethininbox");
            m_square->initPointMove(pointTouch);
        }
        else{
            WDebug(debugSquare, "TabletCanvas" << __FUNCTION__ << "not in box");
            m_square->initPoint(pointTouch);
        }
    }else if(rubber_method){
        m_rubber->initRubber(event->posF());
    }

    m_deviceDown = true;
    lastPoint.pos = event->pos();
    lastPoint.pressure = event->pressure();
}

force_inline void TabletCanvas::ManageMove(
        QTabletEvent    *event,
        const QPointF   &point)
{
    QPainter painter;
    constexpr not_used bool debugMove = true;

    if(event->deviceType() == QTabletEvent::RotationStylus){
        updateCursor(event);
    }

    if(unlikely(!m_deviceDown))
        return;

    painter.begin(&m_pixmap);
    W_ASSERT(painter.isActive());

    if(likely(insert_method)){
        updateBrush(event);
    }

    lastPoint.pos = event->pos();
    lastPoint.pressure = event->pressure();

    if(likely(insert_method)){
        updatelist(event);
        m_finder->move();
    }
    else if(rubber_method){
        m_rubber->actionRubber(point);
    }
    else if(selection_method){
        m_square->isMoving();

        if(m_square->somethingInBox()){
            DO_IF_DEBUG(
                if(m_square->get_first_point().isNotSet())
                            std::abort();
            );

            /* a questo punto può muovere di un delta x e y */
            m_square->move(point);
        }else{
            /*
            * it means that the user not select anything
            * in the past
            */
            m_square->updatePoint(point);
        }
    }else if(text_method){
        if(m_text_w->isIn(point)){
            
        }
        else{
            m_text_w->createNew(point);
        }
    }
}

force_inline void TabletCanvas::ManageFinish(QTabletEvent *event, cbool isForce)
{
    bool done = m_square->somethingInBox();
    block_scrolling = false;

#if defined(WIN32) || defined(WIN64)
    this->isdrawing = false;
#endif

    if(likely(m_redoundo)){
        m_redoundo->copy();
    }

    if(likely(insert_method)){
        AppendAll(*this->data, this, this->medotodiinserimento);
        m_finder->endMoving();
    }

    if(unlikely(!m_deviceDown)){
        if(selection_method && done){
            m_square->reset();
        }
    }

    if (m_deviceDown && event->buttons() == Qt::NoButton){
        m_deviceDown = false;

        if(selection_method){
            if(!done){
                m_square->find();
            }
            if(!isForce)
                m_square->endMoving(this);

        }else if(rubber_method){
            m_rubber->endRubber();
        }
    }
}

void TabletCanvas::updatelist(QTabletEvent *event)
{
    double size;
    uchar alfa;
    point_s tmp_point;
    stroke &strokeTmp = __tmp;
    cbool highlighter = CHECK_FLAG(medotodiinserimento, highlighter);
    const QPointF &pointTouch = event->posF();

    size = event->pressure();
    alfa = unlikely(highlighter) ? m_highlighter->getAlfa() : 255;

    if(!this->m_deviceDown){
        strokeTmp.setPositioneAudio(parent->m_audio_recorder->getCurrentTime());
        strokeTmp.setColor(m_color);
        strokeTmp.setAlfaColor(alfa);
    }
    else{
        /* it's already set */
    }

    tmp_point._x = pointTouch.x();
    tmp_point._y = pointTouch.y();
    tmp_point.pressure = unlikely(highlighter) ? m_highlighter->getSize(size) : m_pen_ui->getSize(size);

    strokeTmp.append(tmp_point);
}
