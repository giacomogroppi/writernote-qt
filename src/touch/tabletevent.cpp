#include "tabletcanvas.h"
#include <QDebug>
#include "../utils/dialog_critic/dialog_critic.h"

bool need_save_auto = false;
bool need_save_tmp = false;

void TabletCanvas::tabletEvent(QTabletEvent *event){
    static bool sel;
    static bool highlighter;
    static bool pen;
    static bool selection;
    static bool rubber;
    static bool text;
    static QEvent::Type eventType;
    const QPointF& pointTouch = event->posF();
    static double DocHeight, DocWidth;

    DocHeight = data->datatouch->biggery();
    DocWidth = data->datatouch->biggerx();

    isWriting = true;
    need_save_auto = true;
    need_save_tmp = true;

    sel = true;
    highlighter = (medotodiinserimento == e_method::highlighter);
    pen = (medotodiinserimento == e_method::pen);
    selection = (medotodiinserimento == e_method::selection);
    rubber = (medotodiinserimento == e_method::rubber);
    text = (medotodiinserimento == e_method::text);

    eventType = event->type();

    qDebug() << "QEvent::Type" << event->deviceType();

    if(pointTouch.x() > DocWidth || pointTouch.y() > DocHeight){
        /* the user is writing in a part where the sheet is not present. You don't have to save the point. And save the end of the current treatment */
        ManageFinish(sel, selection, highlighter, event);
        goto end;
    }

    switch (eventType) {
        case QEvent::TabletPress: /* when the user release the tablet */
            if (!m_deviceDown) {
                if(pen || highlighter){
                    updatelist(event);
                }
                else if(selection){
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

                if(highlighter)
                    this->m_highlighter->setId(this->data->datatouch->lastId());

            }
            break;
        case QEvent::TabletMove: /* user move the pen */
            if (event->device() == QTabletEvent::RotationStylus)
                updateCursor(event);

#if defined(WIN32) || defined(WIN64)
        this->isdrawing = true;
#endif
            if (m_deviceDown) {
                QPainter painter(&m_pixmap);
                if(pen || highlighter){
                    updateBrush(event);

                    paintPixmap(painter, event);
                }

                lastPoint.pos = event->pos();
                lastPoint.pressure = event->pressure();
                lastPoint.rotation = event->rotation();

                if(pen || highlighter){
                    updatelist(event);
                }
                else if(rubber)
                    m_rubber->actionRubber(data->datatouch, pointTouch, painter);
                else if(selection){
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
                }else if(text){
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
            this->ManageFinish(sel, selection, highlighter, event);
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


inline void TabletCanvas::ManageFinish(bool &sel, const bool &selection, const bool &highlighter, QTabletEvent *event){
#if defined(WIN32) || defined(WIN64)
            this->isdrawing = false;
#endif
    if(m_redoundo)
        m_redoundo->copy();

    if (m_deviceDown && event->buttons() == Qt::NoButton){
        m_deviceDown = false;
        if(selection){
            sel = false;

        if(!m_square.check)
            m_square.find(data);
        }

    else if(m_rubber->m_type_gomma == rubber_ui::total){
        m_rubber->clearList(data->datatouch);
    }

    if(highlighter)
        m_highlighter->moveAll(data->datatouch);
    }
}
