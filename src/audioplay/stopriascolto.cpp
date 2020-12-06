#ifndef STOP_RIASCOLTO_CPP
#define STOP_RIASCOLTO_CPP

#include "../mainwindow.h"
#include "ui_mainwindow.h"

#include "../self_class.h"


/* gestisce lo stop dell'ascolto dell'audio */
void MainWindow::on_stopriascolto_clicked()
{
    this->player->setPosition(0);
    this->ui->textEdit->setHtml(this->self->currenttitle.testi);

    this->ui->audioSlider->setSliderPosition(0);
}


#endif // STOP_RIASCOLTO_CPP
