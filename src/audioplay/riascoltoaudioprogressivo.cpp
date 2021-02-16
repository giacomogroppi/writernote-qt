#include "../mainwindow.h"
#include "ui_mainwindow.h"
#include "../self_class.h"

/* funzione che gestiste il riascolto dell'audio */
void MainWindow::riascoltoaudioprogressivo(qint64 position){
    if(this->player->state() != QMediaPlayer::PlayingState)
        return;

    /* scrittura */
    if(self->currenttitle.posizione_binario == ""){
       int position_inlist = this->self->currenttitle.posizione_iniz.indexOf(position/1000);
       if(position_inlist == -1) return;

       int lenght = this->self->currenttitle.testinohtml.at(position_inlist).length();
       QString testoGrassetto = "<!DOCTYPE html><html><body><b>" + this->self->currenttitle.testinohtml[position_inlist] + "</b>";
       testoGrassetto += self->currenttitle.testinohtml.last().mid(lenght, -1) + "</body></html>";

       this->ui->textEdit->setHtml(testoGrassetto);
    }
    else {
        /* parte responsabile della penna */
        int position_inlist = this->self->currenttitle.datatouch->posizioneaudio.indexOf(position/1000);
        if(position_inlist == -1) return;

        this->m_canvas->riascolto(position_inlist);
    }

    this->ui->audioSlider->blockSignals(true);

    this->ui->audioSlider->setValue((position*100)/this->player->duration());

    this->ui->audioSlider->blockSignals(false);


    this->ui->statusBar->showMessage(tr("%1 second").arg(position / 1000));
}
