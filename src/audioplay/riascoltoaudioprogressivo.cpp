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


#ifdef STAMPA
    qDebug() << "position" << position;
#endif

    int position_inlist = this->self->currenttitle.posizione_iniz.indexOf(position);

    if(position_inlist == -1) return;

    //int lung = this->self->currenttitle.testinohtml[position_inlist].length();

    QString testoGrassetto = "<!DOCTYPE html><html><body><b>" + this->self->currenttitle.testinohtml[position_inlist] + "</b>";
    testoGrassetto += self->currenttitle.testinohtml.last().mid(position_inlist,-1) + "</body></html>";

    this->ui->textEdit->setHtml(testoGrassetto);

    this->ui->audioSlider->blockSignals(true);

    if (this->player->duration() != 0)
        this->ui->audioSlider->setValue(position/this->player->duration()*100);

    this->ui->audioSlider->blockSignals(false);
}

#endif
