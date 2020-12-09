#include "../mainwindow.h"
#include "ui_mainwindow.h"

#include <QDebug>

void MainWindow::on_audioSlider_valueChanged(int value)
{
    qDebug() << "Duretion change " << value;
    this->player->setPosition(value*1000);
}
