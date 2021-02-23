#include "tabletcanvas.h"
#include "method/methoddefinition.h"

#include "../utils/dialog_critic/dialog_critic.h"

/* funzione che viene richiamata tutte le volte che si muove qualcosa sulla tabella */
void TabletCanvas::tabletEvent(QTabletEvent *event){
    switch (event->type()) {
        case QEvent::TabletPress:
            if (!m_deviceDown) {
                if(this->medotodiinserimento == STILO)
                    updatelist(event);

                else if(medotodiinserimento == SELEZIONE)
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
        case QEvent::TabletMove:
        if (event->device() == QTabletEvent::RotationStylus)
                        updateCursor(event);

#if defined(WIN32) || defined(WIN64)
        this->isdrawing = true;
#endif

            if (m_deviceDown) {
                QPainter painter(&m_pixmap);
                if(this->medotodiinserimento == STILO){
                    updateBrush(event);
                    paintPixmap(painter, event);
                }

                /* se non è settato in STILO non bisogna aggiornare la lista ne scrivere */
                lastPoint.pos = event->pos();
                lastPoint.pressure = event->pressure();
                lastPoint.rotation = event->rotation();

                if(this->medotodiinserimento == STILO){
                    updatelist(event);
                }
                else if(medotodiinserimento == GOMMA){
                    gomma(painter);
                    if(lastPoint.pos.x() - m_rubber->m_size_gomma >= 0
                            && lastPoint.pos.x() + m_rubber->m_size_gomma < this->m_pixmap.width()
                            && lastPoint.pos.y() - m_rubber->m_size_gomma >= 0
                            && lastPoint.pos.y() + m_rubber->m_size_gomma < this->m_pixmap.height())
                        update(QRect(lastPoint.pos.x() - m_rubber->m_size_gomma,
                                     lastPoint.pos.y() - m_rubber->m_size_gomma,
                                     lastPoint.pos.x() + m_rubber->m_size_gomma,
                                     lastPoint.pos.y() + m_rubber->m_size_gomma));
                    else
                        update();

                }
                else if(medotodiinserimento == SELEZIONE){
                    if(!this->square_.check){
                        isloading = true;
                        square_.disegno(painter, event->pos());

                        update(QRect(QPoint(0, 0), QPoint(m_pixmap.width(), m_pixmap.height())));
                    }
                    else{
                        /* capisce se il tocco è stato interno */
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
                }
            }
            break;
        case QEvent::TabletRelease:
#if defined(WIN32) || defined(WIN64)
            this->isdrawing = true;
#endif

            /* last save and undo redo */
            if(m_redoundo)
                this->m_redoundo->copy();

            if(m_autosave)
                if(!this->m_autosave->save())
                    dialog_critic("We had a problem saving the copybook");

            if (m_deviceDown && event->buttons() == Qt::NoButton){
                m_deviceDown = false;
                if(medotodiinserimento == SELEZIONE){
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
                    this->gomma_delete();
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
