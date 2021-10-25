#include "../../mainwindow.h"
#include "ui_mainwindow.h"

void MainWindow::on_sliderZoom_sliderMoved(int position)
{
    double newZoom = double(position)/100.0;
    //if(newZoom < 1.0)
    //    newZoom = newZoom * 2;

    this->m_currenttitle->datatouch->zoom = newZoom;
    this->m_canvas->zoomChange();
    this->ui->labelZoom->setText(QString("Zoom %1%").arg(position));
}
