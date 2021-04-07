#include "tabletcanvas.h"
#include <QDebug>
#include "../utils/dialog_critic/dialog_critic.h"

/* funzione che viene richiamata tutte le volte che si muove qualcosa sulla tabella */
void TabletCanvas::tabletEvent(QTabletEvent *event){
    isWriting = true;
    bool check;

    auto eventType = event->type();

    switch (eventType) {
        case QEvent::TabletPress: /* when the user release the tablet */
            if (!m_deviceDown) {
                if(this->medotodiinserimento == e_method::pen){
                    updatelist(event);
                }
                else if(medotodiinserimento == e_method::selection){
                    square_.updatePoint(event->posF());
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
                    if(!this->square_.check){ /* it means that the user not select anything */
                        square_.updatePoint(event->posF());
                        isloading = true;
                        update();
                    }
                    else{
                        if(!this->square_.isinside(event->posF())){
                            /* se il tocco non è stato interno */
                            this->square_.reset();
                            isloading = true;
                            update();
                        }
                        else{
                            /* a questo punto può muovere di un delta x e y */
                            this->square_.move(event->posF(), painter, data->datatouch);

                            isloading = true;
                            update();
                        }
                    }
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
                    if(this->square_.check){
                        square_.adjustPoint();
                    }
                    else{

                        check = this->square_.find(data->datatouch);

                        if(check){
                            /* if he find something to move */
                        }

                        isloading = true;

                        update();
                    }
                }

                else if(m_rubber->m_type_gomma == rubber_ui::total){
                    this->m_rubber->clearList(data->datatouch);
                }

                isloading = true;
                update();
            }
            break;
        default:
            break;
    }
    event->accept();
}
