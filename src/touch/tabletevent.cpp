#include "tabletcanvas.h"
#include "method/methoddefinition.h"

#include <QDebug>

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
                        //qDebug() << "Vero ";
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
            /* richiamata quando la penna scorre toccando lo schermo */
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
                        }
                        else{
                            /* a questo punto può muovere di un delta x e y */
                            QRect recttemp = this->square_.move(event->pos());
                            isloading = true;
                            if(!(recttemp == QRect(-1, -1, -1, -1)))
                                update(QRect(QPoint(0, 0), QPoint(m_pixmap.width(), m_pixmap.height())));
                        }
                    }
                }
            }
            break;
        case QEvent::TabletRelease:
            if (m_deviceDown && event->buttons() == Qt::NoButton){
                m_deviceDown = false;
                if(medotodiinserimento == SELEZIONE){
                    this->square_.setData(this->data);
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
                else
                    update();
                isloading = true;
                update();
            }
            break;
        default:
            break;
    }
    event->accept();
}
