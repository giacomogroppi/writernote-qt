#include "tabletcanvas.h"
#include "../mainwindow.h"

#include "ui_mainwindow.h"

/* penna */
void MainWindow::on_actionpen_triggered()
{
    this->m_canvas->medotodiinserimento = 0;
}

/* gomma */
void MainWindow::on_actionrubber_triggered()
{
    this->m_canvas->medotodiinserimento = 1;

}



void TabletCanvas::sceltacolorepenna(QColor color){
    this->m_color = color;
    this->m_pen.setColor(this->m_color);
}


void MainWindow::on_actionblack_triggered()
{
    this->m_canvas->sceltacolorepenna(Qt::black);
}

void MainWindow::on_actionwhite_triggered()
{
    this->m_canvas->sceltacolorepenna(Qt::white);
}

void MainWindow::on_actionyellow_triggered()
{
    this->m_canvas->sceltacolorepenna(Qt::yellow);
}

void MainWindow::on_actionred_triggered()
{
    this->m_canvas->sceltacolorepenna(Qt::red);
}
