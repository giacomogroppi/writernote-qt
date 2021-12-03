#include "../../mainwindow.h"
#include "ui_mainwindow.h"

#define UPDATE_TEXT(position) this->ui->labelZoom->setText(QString("Zoom %1%").arg(position));

void MainWindow::on_sliderZoom_sliderMoved(int position)
{
    double newZoom = double(position)/100.0;

    m_canvas->data->datatouch->changeZoom(newZoom, this->m_canvas);
    UPDATE_TEXT(position);
}

void MainWindow::zoomChange()
{
    this->ui->sliderZoom->setSliderPosition(m_canvas->data->datatouch->getZoom() * 100);
    UPDATE_TEXT(this->m_canvas->data->datatouch->getZoom() * 100);
}
