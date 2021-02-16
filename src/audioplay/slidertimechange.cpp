#include "../mainwindow.h"
#include "ui_mainwindow.h"

void MainWindow::on_audioSlider_valueChanged(int value)
{
    this->player->setPosition(value*1000);
}
