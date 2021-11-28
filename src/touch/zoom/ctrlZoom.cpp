#include "../../mainwindow.h"
#include "ui_mainwindow.h"

void MainWindow::on_sliderZoom_sliderMoved(int position)
{
    double newZoom = double(position)/100.0;

    m_canvas->data->datatouch->changeZoom(newZoom, this->m_canvas);
    this->ui->labelZoom->setText(QString("Zoom %1%").arg(position));
}

void MainWindow::zoomChange()
{
    this->on_sliderZoom_sliderMoved(m_canvas->data->datatouch->getZoom()*100);
}
