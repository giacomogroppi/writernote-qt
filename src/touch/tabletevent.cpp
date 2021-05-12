#include "tabletcanvas.h"
#include <QDebug>
#include "../utils/dialog_critic/dialog_critic.h"

bool need_save_auto = false;
bool need_save_tmp = false;

/* funzione che viene richiamata tutte le volte che si muove qualcosa sulla tabella */
void TabletCanvas::tabletEvent(QTabletEvent *event){
    isWriting = true;
    need_save_auto = true;
    need_save_tmp = true;

    bool sel = true;
    bool check;

    auto eventType = event->type();

    switch (eventType) {
        case QEvent::TabletPress: /* when the user release the tablet */
            if (!m_deviceDown) {
                if(this->medotodiinserimento == e_method::pen){
                    updatelist(event);
                }
                else if(medotodiinserimento == e_method::selection){
                    m_square.updatePoint(event->posF());
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
                if(this->medotodiinserimento == e_method::pen){
                    updateBrush(event);

                    paintPixmap(painter, event);
                }

                lastPoint.pos = event->pos();
                lastPoint.pressure = event->pressure();
                lastPoint.rotation = event->rotation();

                if(this->medotodiinserimento == e_method::pen){
                    updatelist(event);
                }
                else if(medotodiinserimento == e_method::rubber){

                    if(m_rubber->actionRubber(data->datatouch, event->posF(), painter))
                        isloading = true;

                }
                else if(medotodiinserimento == e_method::selection){
                    if(!m_square.check){ /* it means that the user not select anything */
                        m_square.updatePoint(event->posF());
                        isloading = true;
                    }
                    else{
                        if(!m_square.isinside(event->posF())){
                            /* se il tocco non è stato interno */
                            m_square.reset();
                            isloading = true;
                        }
                        else{
                            /* a questo punto può muovere di un delta x e y */
                            m_square.move(event->posF(), painter, data->datatouch);

                            isloading = true;
                        }
                    }
                    sel = false;
                }else if(medotodiinserimento == e_method::text){
                    if(m_text_w->isIn(event->posF())){

                    }
                    else{
                        m_text_w->createNew(event->posF());
                    }
                }
            }
            break;
        case QEvent::TabletRelease: /* pen leaves the tablet */

#if defined(WIN32) || defined(WIN64)
            this->isdrawing = false;
#endif

            /* last save and undo redo */
            if(m_redoundo)
                this->m_redoundo->copy(this->data);

            if(m_autosave)
                if(!this->m_autosave->save())
                    dialog_critic("We had a problem saving the copybook");

            if (m_deviceDown && event->buttons() == Qt::NoButton){
                m_deviceDown = false;
                if(medotodiinserimento == e_method::selection){
                    if(m_square.check){
                        m_square.adjustPoint();
                    }
                    else{

                        check = m_square.find(data->datatouch);

                        if(check){
                            /* if he find something to move */
                        }

                        isloading = true;

                    }
                }

                else if(m_rubber->m_type_gomma == rubber_ui::total){
                    this->m_rubber->clearList(data->datatouch);
                }

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
