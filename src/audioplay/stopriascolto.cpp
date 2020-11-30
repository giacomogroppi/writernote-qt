#ifndef STOP_RIASCOLTO_CPP
#define STOP_RIASCOLTO_CPP

#include "../mainwindow.h"
#include "ui_mainwindow.h"

#include "../self_class.h"

void MainWindow::on_stopriascolto_clicked()
{
    this->player->setPosition(0);
    ui->textEdit->setHtml(this->self->currenttitle.testi);

    this->ui->audioSlider->setSliderPosition(0);
}


#endif // STOP_RIASCOLTO_CPP
