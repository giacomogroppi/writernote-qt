#include "../../mainwindow.h"
#include "ui_mainwindow.h"

void MainWindow::on_sliderZoom_sliderMoved(int position)
{
    double newZoom = double(position)/100.0;

    m_currenttitle->datatouch->changeZoom(newZoom, this->m_canvas);
    this->ui->labelZoom->setText(QString("Zoom %1%").arg(position));
}
