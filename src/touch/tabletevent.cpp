#include "tabletcanvas.h"
#include <QDebug>
#include "../utils/dialog_critic/dialog_critic.h"

bool need_save_auto = false;
bool need_save_tmp = false;

void TabletCanvas::tabletEvent(QTabletEvent *event){
    isWriting = true;
    need_save_auto = true;
    need_save_tmp = true;

    bool sel = true;
    bool highlighter = medotodiinserimento == e_method::highlighter;

    auto eventType = event->type();

    //qDebug() << eventType;

    switch (eventType) {
        case QEvent::TabletPress: /* when the user release the tablet */
            if (!m_deviceDown) {
                if(medotodiinserimento == e_method::pen
                        || highlighter){
                    updatelist(event);
                }
                else if(medotodiinserimento == e_method::selection){
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
                if(medotodiinserimento == e_method::pen
                        || highlighter){
                    updateBrush(event);

                    paintPixmap(painter, event);
                }

                lastPoint.pos = event->pos();
                lastPoint.pressure = event->pressure();
                lastPoint.rotation = event->rotation();

                if(highlighter){
                    m_highlighter->append(data->datatouch->length());
                }

                if(this->medotodiinserimento == e_method::pen
                        || highlighter){
                    updatelist(event);
                }
                else if(medotodiinserimento == e_method::rubber){

                    if(m_rubber->actionRubber(data->datatouch, event->posF(), painter))
                        isloading = true;

                }
                else if(medotodiinserimento == e_method::selection){
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
                    isloading = true;
                    sel = false;
                }else if(medotodiinserimento == e_method::text){
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
                this->m_redoundo->copy(this->data);

            if (m_deviceDown && event->buttons() == Qt::NoButton){
                m_deviceDown = false;
                if(medotodiinserimento == e_method::selection){
                    sel = false;
                    m_square.adjustPoint();

                    if(!m_square.check){
                        m_square.find(data);

                        isloading = true;

                    }
                }

                else if(m_rubber->m_type_gomma == rubber_ui::total){
                    this->m_rubber->clearList(data->datatouch);
                }

                m_highlighter->moveAll(data->datatouch);

                isloading = true;
            }
            break;
        default:
            break;
    }

    if(sel){
        m_square.reset();
    }

    if(isloading)
        update();

    event->accept();
}
