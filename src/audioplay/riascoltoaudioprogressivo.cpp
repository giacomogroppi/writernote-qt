#include "../mainwindow.h"
#include "ui_mainwindow.h"

#include "../utils/dialog_critic/dialog_critic.h"

#define UPDATE_S(x) ui->statusBar->showMessage(tr("%1 second").arg(x / 1000));
#include <QRunnable>
#include <QTimer>
#include <QThreadPool>

static int p_index;
static double p_time;
static int p_count;

static int current_p_index = 0;

void MainWindow::showRiascolto(){
    int somma = p_index + current_p_index;

    int lenght = m_currenttitle->testinohtml.at(somma).length();
    QString testoGrassetto = "<!DOCTYPE html><html><body><b>" + m_currenttitle->testinohtml.at(somma) + "</b>";
    testoGrassetto += m_currenttitle->testinohtml.last().mid(lenght, -1) + "</body></html>";

    this->ui->textEdit->setHtml(testoGrassetto);

    current_p_index ++;

    if(current_p_index * p_time < 1){
        this->m_timer->start(p_time*1000);
    }
    else
        this->m_timer->stop();
}


/* funzione che gestiste il riascolto dell'audio
 * viene richiamata quando l'audio viene riprodotto
*/
void MainWindow::riascoltoaudioprogressivo(qint64 position){
    if(this->player->state() != QMediaPlayer::PlayingState)
        return;

    UPDATE_S(position)

    /* keyboard */
    if(!m_currenttitle->m_touch){
        p_index = m_currenttitle->posizione_iniz.indexOf(position/1000);
        if(p_index == -1)
            return;

        current_p_index = 0;

        p_count = m_currenttitle->posizione_iniz.count(position/1000);

        p_time = 1/(double)p_count;

        this->m_timer->start(p_time*1000);
    }
    else {
        /*
        int position_inlist = 0, i, len, position_temp;

        len = m_currenttitle->datatouch->length();
        for(i=0, position_temp = position/1000; i < len; i++){
            if(m_currenttitle->datatouch->at(i)->m_posizioneaudio == position_temp){
                position_inlist = i;
                break;
            }
        }

        if(position_inlist == -1) return;
        */
        this->m_canvas->riascolto(position/1000);
    }

    this->ui->audioSlider->blockSignals(true);

    int duration = player->duration();
    if(!duration)
        return user_message("The audio seems blank");

    this->ui->audioSlider->setValue((position*100)/duration);

    this->ui->audioSlider->blockSignals(false);

}
