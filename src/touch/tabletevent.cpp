#include "tabletcanvas.h"
#include <QDebug>
#include "utils/dialog_critic/dialog_critic.h"
#include "square/square.h"
#include "mainwindow.h"
#include "audioplay/audioplay.h"
#include "audiorecord/audiorecord.h"

#define MAXPOINT 20

stroke __tmp;
extern bool block_scrolling;
static void AppendAll(Document &doc, const TabletCanvas *canvas, const bool toTheTop);

bool need_save_auto = false;
bool need_save_tmp = false;

static double DocHeight, DocWidth;

static bool sel;
static bool highlighter_method;
static bool pen_method;
static bool selection_method;
static bool rubber_method;
static bool text_method;

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

    DocHeight = data->datatouch->biggery();
    DocWidth = data->datatouch->biggerx();

    isWriting = true;
    need_save_auto = true;
    need_save_tmp = true;
    sel = true;
    block_scrolling = true;

    eventType = event->type();

    rubber_method = medotodiinserimento == e_method::rubber || event->pointerType() == QTabletEvent::PointerType::Eraser;
    if(rubber_method){
        setFalse();
    }
    else{
        highlighter_method = medotodiinserimento == e_method::highlighter;
        pen_method = medotodiinserimento == e_method::pen;
        selection_method = medotodiinserimento == e_method::selection;
        text_method = medotodiinserimento == e_method::text;
    }

    if(pointTouch.x() > DocWidth || pointTouch.y() > DocHeight){
        /* the user is writing in a part where the sheet is not present. You don't have to save the point. And save the end of the current treatment */
        ManageFinish(event);
        goto end;
    }

    if(eventType == QEvent::TabletPress){ /* first point */
        ManageStart(event, pointTouch);
    }
    else if(eventType == QEvent::TabletMove){ /* user move the pen */
        if (event->device() == QTabletEvent::RotationStylus){
            updateCursor(event);
        }

        #if defined(WIN32) || defined(WIN64)
        this->isdrawing = true;
        #endif

        if (m_deviceDown) {
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

                if(!m_square->somethingInBox()){ /* it means that the user not select anything */
                    m_square->updatePoint(pointTouch);
                }
                else{
                    if(!m_square->isinside(pointTouch)){
                        /* se il tocco non è stato interno */
                        m_square->reset();
                    }
                    else{
                        /* a questo punto può muovere di un delta x e y */
                        m_square->move(pointTouch);
                    }
                }
                sel = false;
            }else if(text_method){
                if(m_text_w->isIn(pointTouch)){

                }
                else{
                    m_text_w->createNew(pointTouch);
                }
            }
        }else{
            sel = false;
        }
    }

    else if(eventType == QEvent::TabletRelease){ /* pen leaves the tablet */
        this->ManageFinish(event);
    }

end:

    if(sel){
        m_square->reset();
    }
    if(!sel && selection_method){
        data->datatouch->triggerViewIfVisible(parent->m_audioplayer->getPositionSecond());
    }

    update();

    event->accept();
}


inline void TabletCanvas::ManageFinish(QTabletEvent *event){
#if defined(WIN32) || defined(WIN64)
    this->isdrawing = false;
#endif
    if(m_redoundo)
        m_redoundo->copy();

    if(pen_method || highlighter_method)
        AppendAll(*this->data, this, (highlighter_method) ? m_highlighter->moveToTop() : false);

    if (m_deviceDown && event->buttons() == Qt::NoButton){
        m_deviceDown = false;
        if(selection_method){
            sel = false;

            if(!m_square->somethingInBox())
                m_square->find(data);
            m_square->endMoving(this);
        }else if(rubber_method){
            m_rubber->endRubber(data->datatouch);
        }
    }
    block_scrolling = false;
}

Q_ALWAYS_INLINE void TabletCanvas::ManageStart(
        QTabletEvent    *event,
        const QPointF   &pointTouch)
{
    if(m_deviceDown)
        return;

    if(pen_method || highlighter_method){
        updatelist(event);
    }
    else if(selection_method){
        if(m_square->somethingInBox()){
            m_square->move(pointTouch);
        }
        else{
            m_square->updatePoint(pointTouch);
        }
        sel = false;
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

static void AppendAll(Document &doc, const TabletCanvas *canvas, const bool toTheTop){
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
