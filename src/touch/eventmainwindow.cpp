#include "../mainwindow.h"
#include "ui_mainwindow.h"

#include <QDebug>

/* la funzione gestisce l'inquadramento del pixmap con la dimensione degli spacer */
bool MainWindow::event(QEvent *event){
    qDebug() << event->type();

    /*if(event->type() != QEvent::Resize && event->type() != QEvent::UpdateRequest)
        if(!this->m_canvas->iszoomin && !this->m_canvas->iszoomin)
            return QWidget::event(event);

    if(m_canvas->width() < NUMEROPIXELORIZZONALI){
        this->ui->spacersinistraeditor->changeSize(0, 0);
    }
    else{
        qDebug() << "Entra";
        int a;
        a = NUMEROPIXELORIZZONALI - m_canvas->width();
        this->ui->spacersinistraeditor->changeSize(500, 500);
        layout()->invalidate();
    }

    */

    return QWidget::event(event);
}
