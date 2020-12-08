#ifndef STOP_RIASCOLTO_CPP
#define STOP_RIASCOLTO_CPP

#include "../mainwindow.h"
#include "ui_mainwindow.h"

#include "../self_class.h"


/* gestisce lo stop dell'ascolto dell'audio */
void MainWindow::on_stopriascolto_clicked()
{
    this->on_actionListen_current_audio_triggered();
}


#endif // STOP_RIASCOLTO_CPP
