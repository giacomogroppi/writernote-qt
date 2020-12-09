#include "../mainwindow.h"
#include "ui_mainwindow.h"

void MainWindow::on_volumeSlider_valueChanged(int value)
{
    this->player->setVolume(value);
}
