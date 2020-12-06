#ifndef RIASCOLTO_AUDIO_PROGRESSIVO_CPP
#define RIASCOLTO_AUDIO_PROGRESSIVO_CPP

#include "../mainwindow.h"
#include "ui_mainwindow.h"
#include "QDebug"
#include "../self_class.h"

/* funzione che gestiste il riascolto dell'audio */
void MainWindow::riascoltoaudioprogressivo(qint64 position){
    if(this->player->state() != QMediaPlayer::PlayingState)
        return;

    int position_inlist = this->self->currenttitle.posizione_iniz.indexOf(position/1000);

    if(position_inlist == -1) return;

    int lenght = this->self->currenttitle.testinohtml.at(position_inlist).length();
    QString testoGrassetto = "<!DOCTYPE html><html><body><b>" + this->self->currenttitle.testinohtml[position_inlist] + "</b>";
    testoGrassetto += self->currenttitle.testinohtml.last().mid(lenght, -1) + "</body></html>";

    this->ui->textEdit->setHtml(testoGrassetto);

    this->ui->audioSlider->blockSignals(true);

    this->ui->audioSlider->setValue((position*100)/this->player->duration());

    this->ui->audioSlider->blockSignals(false);


    this->ui->statusBar->showMessage(tr("%1 second").arg(position / 1000));
}

#endif
