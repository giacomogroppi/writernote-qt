#include "../mainwindow.h"

/* gestisce il settaggio del volume */
void MainWindow::on_volumeSlider_actionTriggered(int action)
{
    this->player->setVolume(action);
}
