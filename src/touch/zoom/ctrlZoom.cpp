#include "mainwindow.h"
#include "ui_mainwindow.h"

#define UPDATE_TEXT(position) this->ui->labelZoom->setText(QString("Zoom %1%").arg(int(position)));

void MainWindow::on_sliderZoom_sliderMoved(int position)
{
    double newZoom = double(position)/100.0;

    _canvas->getDoc()->datatouch->changeZoom(newZoom, _canvas);
    UPDATE_TEXT(position);
}

void MainWindow::zoomChange()
{
    this->ui->sliderZoom->setSliderPosition(_canvas->getDoc()->datatouch->getZoom() * 100);
    UPDATE_TEXT(_canvas->getDoc()->datatouch->getZoom() * 100);
}
