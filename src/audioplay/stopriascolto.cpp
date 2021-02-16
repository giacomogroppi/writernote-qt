#include "../mainwindow.h"
#include "ui_mainwindow.h"


/* gestisce lo stop dell'ascolto dell'audio */
void MainWindow::on_stopriascolto_clicked()
{
    this->on_actionListen_current_audio_triggered();
}
