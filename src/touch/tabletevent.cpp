#include "tabletcanvas.h"
#include "method/methoddefinition.h"


#include "../utils/dialog_critic/dialog_critic.h"

/* funzione che viene richiamata tutte le volte che si muove qualcosa sulla tabella */
void TabletCanvas::tabletEvent(QTabletEvent *event){
    isWriting = true;

    switch (event->type()) {
        case QEvent::TabletPress: /* first point touch */
            if (!m_deviceDown) {
                if(this->medotodiinserimento == METHOD_STILO)
                    updatelist(event);

                else if(medotodiinserimento == METHOD_SELEZIONE)
                {
                    if(square_.check){
                        square_.lastpoint = event->pos();
                    }
                    else
                        square_.pointinit = event->pos();
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
                if(this->medotodiinserimento == METHOD_STILO){
                    updateBrush(event);

                    paintPixmap(painter, event);
                }

                lastPoint.pos = event->pos();
                lastPoint.pressure = event->pressure();
                lastPoint.rotation = event->rotation();

                if(this->medotodiinserimento == METHOD_STILO){
                    updatelist(event);
                }
                else if(medotodiinserimento == METHOD_GOMMA){

                    if(m_rubber->actionRubber(data->datatouch, event->posF(), painter))
                        update();

                }
                else if(medotodiinserimento == METHOD_SELEZIONE){
                    if(!this->square_.check){ /* it means that he has not learned anything */
                        isloading = true;
                        square_.disegno(painter, event->pos());

                        update(QRect(QPoint(0, 0), QPoint(m_pixmap.width(), m_pixmap.height())));
                    }
                    else{
                        if(!this->square_.isinside(event->pos())){
                            /* se il tocco non è stato interno */
                            this->square_.reset();
                            isloading = true;
                            update(QRect(square_.pointinit.toPoint(), square_.pointfine.toPoint()));
                        }
                        else{
                            /* a questo punto può muovere di un delta x e y */
                            this->square_.move(event->pos(), painter);
                            isloading = true;
                            update(QRect(QPoint(0, 0), QPoint(m_pixmap.width(), m_pixmap.height())));
                        }
                    }
                }else if(medotodiinserimento == METHOD_TEXT){
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
            this->isdrawing = true;
#endif

            /* last save and undo redo */
            if(m_redoundo)
                this->m_redoundo->copy(this->data);

            if(m_autosave)
                if(!this->m_autosave->save())
                    dialog_critic("We had a problem saving the copybook");

            if (m_deviceDown && event->buttons() == Qt::NoButton){
                m_deviceDown = false;
                if(medotodiinserimento == METHOD_SELEZIONE){
                    this->square_.setData(data->datatouch);
                    bool check = this->square_.find();
                    if(!check){
                        isloading = true;
                        update();
                        event->accept();
                        return this->square_.reset();
                    }

                    QPainter painter(&m_pixmap);
                    //update(this->square_.drawsquare(painter));
                    this->m_pixmap.fill(Qt::white);
                    this->square_.drawsquare(painter);
                    isloading = true;

                    update(QRect(QPoint(0, 0), QPoint(m_pixmap.width(), m_pixmap.height())));
                }
                else if(m_rubber->m_type_gomma == TOTALE){
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
