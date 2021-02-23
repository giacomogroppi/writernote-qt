#include "../mainwindow.h"
#include "ui_mainwindow.h"

/* funzione che gestiste il riascolto dell'audio */
void MainWindow::riascoltoaudioprogressivo(qint64 position){
    if(this->player->state() != QMediaPlayer::PlayingState)
        return;

    /* scrittura */
    if(m_currenttitle.m_touch){
       int position_inlist = m_currenttitle.posizione_iniz.indexOf(position/1000);
       if(position_inlist == -1) return;

       int lenght = m_currenttitle.testinohtml.at(position_inlist).length();
       QString testoGrassetto = "<!DOCTYPE html><html><body><b>" + m_currenttitle.testinohtml[position_inlist] + "</b>";
       testoGrassetto += m_currenttitle.testinohtml.last().mid(lenght, -1) + "</body></html>";

       this->ui->textEdit->setHtml(testoGrassetto);
    }
    else {
        /* parte responsabile della penna */
        int position_inlist, i = 0, len, position_temp;

        len = m_currenttitle.datatouch->m_point.length();
        for(i=0, position_temp = position/1000; i < len; i++){
            if(m_currenttitle.datatouch->m_point.at(i).m_posizioneaudio == position_temp){
                position_inlist = i;
                break;
            }
        }

        /*int position_inlist = m_currenttitle.datatouch->posizioneaudio.indexOf(position/1000);*/
        if(position_inlist == -1) return;

        this->m_canvas->riascolto(position_inlist);
    }

    this->ui->audioSlider->blockSignals(true);

    this->ui->audioSlider->setValue((position*100)/this->player->duration());

    this->ui->audioSlider->blockSignals(false);


    this->ui->statusBar->showMessage(tr("%1 second").arg(position / 1000));
}
