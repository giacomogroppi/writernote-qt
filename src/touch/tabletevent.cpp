#include "tabletcanvas.h"
#include <QDebug>
#include "utils/dialog_critic/dialog_critic.h"
#include "square/square.h"
#include "mainwindow.h"
#include "audioplay/audioplay.h"
#include "audiorecord/audiorecord.h"

#define MAXPOINT 20
#define CTRL_METHOD(var, type) var = medotodiinserimento == e_method::type

stroke __tmp;
extern bool block_scrolling;
static void AppendAll(Document &doc, const TabletCanvas *canvas, const bool toTheTop);

bool need_save_auto = false;
bool need_save_tmp = false;

static bool highlighter_method;
static bool pen_method;
static bool selection_method;
static bool rubber_method;
static bool text_method;

static TabletCanvas::e_method lastMethod = TabletCanvas::e_method::pen;

static QEvent::Type eventType;

static Q_ALWAYS_INLINE void setFalse()
{
    highlighter_method = false;
    pen_method = false;
    selection_method = false;
    text_method = false;
}

void TabletCanvas::tabletEvent(QTabletEvent *event){
    const QPointF& pointTouch = event->posF();

    isWriting = true;
    need_save_auto = true;
    need_save_tmp = true;
    block_scrolling = true;

    eventType = event->type();

    CTRL_METHOD(rubber_method, rubber);
    rubber_method |= (event->pointerType() == QTabletEvent::PointerType::Eraser);

    if(unlikely(rubber_method)){
        setFalse();
    }
    else{
        CTRL_METHOD(highlighter_method, highlighter);
        CTRL_METHOD(pen_method, pen);
        CTRL_METHOD(selection_method, selection);
        CTRL_METHOD(text_method, text);
    }

    if(unlikely(    pointTouch.x() > data->datatouch->biggerx()
                ||  pointTouch.y() > data->datatouch->biggery())){
        /* the user is writing in a part where
         *  the sheet is not present.
         * You don't have to save the point. And
         * save the end of the current treatment
         */
        ManageFinish(event);
        goto end;
    }

    if(unlikely(eventType == QEvent::TabletPress)){ /* first point */
        ManageStart(event, pointTouch);
#if defined(WIN32) || defined(WIN64)
        this->isdrawing = true;
#endif
    }
    else if(likely(eventType == QEvent::TabletMove)){ /* user move the pen */
        if (event->deviceType() == QTabletEvent::RotationStylus){
            updateCursor(event);
        }

        if (likely(m_deviceDown)) {
            QPainter painter(&m_pixmap);
            if(pen_method || highlighter_method){
                updateBrush(event);
            }

            lastPoint.pos = event->pos();
            lastPoint.pressure = event->pressure();
            lastPoint.rotation = event->rotation();

            if(pen_method || highlighter_method){
                updatelist(event);
            }
            else if(rubber_method){
                /*const QList<int> &ref = */m_rubber->actionRubber(data->datatouch, pointTouch);

                //if(ref.length()){
                //    this->triggerNewView(ref, true);
                //}
            }
            else if(selection_method){
                m_square->isMoving();

                if(!m_square->somethingInBox()){
                    /*
                     * it means that the user not select anything
                     * in the past
                    */
                    m_square->updatePoint(pointTouch);
                }
                else{
                    if(likely(m_square->isinside(pointTouch))){
                        DO_IF_DEBUG(if(m_square->get_first_point().isNotSet()) std::abort(););
                        /* a questo punto può muovere di un delta x e y */
                        m_square->move(pointTouch);
                    }
                    else{
                        /* se il tocco non è stato interno */
                        m_square->reset();
                    }
                }
            }else if(text_method){
                if(m_text_w->isIn(pointTouch)){

                }
                else{
                    m_text_w->createNew(pointTouch);
                }
            }
        }
    }

    else if(eventType == QEvent::TabletRelease){ /* pen leaves the tablet */
        this->ManageFinish(event);
    }

end:

    if(!selection_method){
        if(lastMethod == e_method::selection){
            m_square->reset();
        }
    }

    update();

    event->accept();
    lastMethod = this->medotodiinserimento;
}


inline void TabletCanvas::ManageFinish(QTabletEvent *event)
{
    bool done = m_square->somethingInBox();
    block_scrolling = false;

#if defined(WIN32) || defined(WIN64)
    this->isdrawing = false;
#endif

    if(likely(m_redoundo)){
        m_redoundo->copy();
    }

    if(pen_method || highlighter_method)
        AppendAll(*this->data, this, (highlighter_method) ? m_highlighter->moveToTop() : false);

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

            m_square->endMoving(this);

        }else if(rubber_method){
            m_rubber->endRubber(data->datatouch);
        }
    }
}

Q_ALWAYS_INLINE void TabletCanvas::ManageStart(
        QTabletEvent    *event,
        const QPointF   &pointTouch)
{
    if(unlikely(m_deviceDown))
        return;

    if(pen_method || highlighter_method){
        updatelist(event);
    }
    else if(selection_method){
        if(m_square->somethingInBox()){
            m_square->initPointMove(pointTouch);
        }
        else{
            m_square->initPoint(pointTouch);
        }
    }

    m_deviceDown = true;
    lastPoint.pos = event->pos();
    lastPoint.pressure = event->pressure();
    lastPoint.rotation = event->rotation();
}

void TabletCanvas::updatelist(QTabletEvent *event)
{
    double size;
    uchar alfa;
    point_s tmp_point;
    stroke &strokeTmp = __tmp;

    const QPointF &pointTouch = event->posF();

    size = event->pressure();
    alfa = highlighter_method ? m_highlighter->getAlfa() : 255;

    if(!this->m_deviceDown){
        strokeTmp.setId(data->datatouch->maxId() + 1);

        strokeTmp.setPositioneAudio(parent->m_audio_recorder->getCurrentTime());
        strokeTmp.setColor(m_color);
        strokeTmp.setAlfaColor(alfa);
    }
    else{
        /* it's already set */
    }

    tmp_point.m_x = pointTouch.x();
    tmp_point.m_y = pointTouch.y();
    tmp_point.pressure = highlighter_method ? m_highlighter->getSize(size) : m_pen_ui->getSize(size);

    strokeTmp.append(tmp_point);
}

static void AppendAll(
        Document             &doc, 
        const TabletCanvas   *canvas, 
        const bool           toTheTop)
{
    uint i;

    /* for debug */
    stroke & strokeToAppend = __tmp;
    int pageMod;
    const uint lenPoint = strokeToAppend.length();
    point_s *point;
    const QPointF &PointFirstPage = doc.datatouch->getPointFirstPage();

    if(!lenPoint) return;

    int time = canvas->parent->m_audioplayer->getPositionSecond();

    for(i = 0; i < lenPoint; i++){
        point = &strokeToAppend.at_mod(i);
        point->m_x -= PointFirstPage.x();
        point->m_y -= PointFirstPage.y();
    }

    pageMod = doc.datatouch->appendStroke(strokeToAppend);

    doc.datatouch->at_mod(pageMod).triggerRenderImage(time, toTheTop);

    strokeToAppend.reset();
}
