#include "tabletcanvas.h"
#include <QDebug>
#include "../utils/dialog_critic/dialog_critic.h"

bool need_save_auto = false;
bool need_save_tmp = false;

static double DocHeight, DocWidth;
static bool highlighter_type;

static bool sel;
static bool highlighter_method;
static bool pen_method;
static bool selection_method;
static bool rubber_method;
static bool text_method;

static QEvent::Type eventType;

void TabletCanvas::tabletEvent(QTabletEvent *event){
    const QPointF& pointTouch = event->posF();
    

    DocHeight = data->datatouch->biggery();
    DocWidth = data->datatouch->biggerx();

    isWriting = true;
    need_save_auto = true;
    need_save_tmp = true;
    sel = true;

    highlighter_type = event->pointerType() == QTabletEvent::PointerType::Eraser;
    eventType = event->type();

    if(event->pointerType() != QTabletEvent::PointerType::Eraser){
        highlighter_method = (highlighter_type) ? true : (medotodiinserimento == e_method::highlighter);

        pen_method = (medotodiinserimento == e_method::pen && !highlighter_type);
        selection_method = (medotodiinserimento == e_method::selection && !highlighter_type);
        rubber_method = (medotodiinserimento == e_method::rubber && !highlighter_type);
        text_method = (medotodiinserimento == e_method::text && !highlighter_type);
    }

    if(pointTouch.x() > DocWidth || pointTouch.y() > DocHeight){
        /* the user is writing in a part where the sheet is not present. You don't have to save the point. And save the end of the current treatment */
        ManageFinish(event);
        goto end;
    }

    switch (eventType) {
        case QEvent::TabletPress: /* when the user release the tablet */
            ManageStart(event, pointTouch);
            break;
        case QEvent::TabletMove: /* user move the pen */
            if (event->device() == QTabletEvent::RotationStylus)
                updateCursor(event);

#if defined(WIN32) || defined(WIN64)
        this->isdrawing = true;
#endif
            if (m_deviceDown) {
                QPainter painter(&m_pixmap);
                if(pen || highlighter_method){
                    updateBrush(event);

                    paintPixmap(painter, event);
                }

                lastPoint.pos = event->pos();
                lastPoint.pressure = event->pressure();
                lastPoint.rotation = event->rotation();

                if(pen_method || highlighter_method){
                    updatelist(event);
                }
                else if(rubber_method)
                    m_rubber->actionRubber(data->datatouch, pointTouch);
                else if(selection_method){
                    if(!m_square.check){ /* it means that the user not select anything */
                        m_square.updatePoint(pointTouch);
                    }
                    else{
                        if(!m_square.isinside(pointTouch)){
                            /* se il tocco non è stato interno */
                            m_square.reset();
                        }
                        else{
                            /* a questo punto può muovere di un delta x e y */
                            m_square.move(pointTouch, data);
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
            break;
        case QEvent::TabletRelease: /* pen leaves the tablet */
            this->ManageFinish(event);
            break;
        default:
            break;
    }

end:

    if(sel){
        m_square.reset();
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

    if (m_deviceDown && event->buttons() == Qt::NoButton){
        m_deviceDown = false;
        if(selection_method){
            sel = false;

        if(!m_square.check)
            m_square.find(data);
        }

    else if(m_rubber->m_type_gomma == rubber_ui::total){
        m_rubber->clearList(data->datatouch);
    }

    if(highlighter_method)
        m_highlighter->moveAll(data->datatouch);
    }
}

inline void TabletCanvas::ManageStart(QTabletEvent *event, const QPointF &pointTouch){
    if(m_deviceDown)
        return;
    if(pen_method || highlighter_method){
        updatelist(event);
    }
    else if(selection_method){
        if(m_square.check){
            m_square.move(pointTouch, data);
        }
        else{
            m_square.updatePoint(pointTouch);
        }
        sel = false;
    }
    m_deviceDown = true;
    lastPoint.pos = event->pos();
    lastPoint.pressure = event->pressure();
    lastPoint.rotation = event->rotation();

    if(highlighter_method)
        this->m_highlighter->setId(this->data->datatouch->lastId());

}
