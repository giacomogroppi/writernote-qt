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

    //qDebug() << "QEvent::Type" << eventType;
    qDebug() << highlighter << pen << selection << rubber << text;

    switch (eventType) {
        case QEvent::TabletPress: /* when the user release the tablet */
            if (!m_deviceDown) {
                if(pen || highlighter){
                    updatelist(event);
                }
                else if(selection){
                    if(m_square.check){
                        m_square.move(event->posF(), data);
                    }
                    else{
                        m_square.updatePoint(event->posF());
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
                    m_rubber->actionRubber(data->datatouch, event->posF(), painter);
                else if(selection){
                    if(!m_square.check){ /* it means that the user not select anything */
                        m_square.updatePoint(event->posF());
                    }
                    else{
                        if(!m_square.isinside(event->posF())){
                            /* se il tocco non è stato interno */
                            m_square.reset();
                        }
                        else{
                            /* a questo punto può muovere di un delta x e y */
                            m_square.move(event->posF(), data);
                        }
                    }
                    sel = false;
                }else if(text){
                    if(m_text_w->isIn(event->posF())){

                    }
                    else{
                        m_text_w->createNew(event->posF());
                    }
                }
            }else{
                sel = false;
            }
            break;
        case QEvent::TabletRelease: /* pen leaves the tablet */
#if defined(WIN32) || defined(WIN64)
            this->isdrawing = false;
#endif

            /*
             * last save and undo redo
            * TODO -> change this call to a timer
            */
            if(m_redoundo)
                this->m_redoundo->copy();

            if (m_deviceDown && event->buttons() == Qt::NoButton){
                m_deviceDown = false;
                if(selection){
                    sel = false;

                    if(!m_square.check)
                        m_square.find(data);
                }

                else if(m_rubber->m_type_gomma == rubber_ui::total){
                    this->m_rubber->clearList(data->datatouch);
                }

                if(highlighter)
                    m_highlighter->moveAll(data->datatouch);

            }
            break;
        default:
            break;
    }

    if(sel){
        m_square.reset();
    }

    update();

    event->accept();
}
