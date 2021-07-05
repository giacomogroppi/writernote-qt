#include "../mainwindow.h"
#include "ui_mainwindow.h"
#include "../utils/dialog_critic/dialog_critic.h"
#include "../setting_ui.h"
#include "cambioiconariascolto.h"

/* la funzione gestisce la gestione del click del punsante con la mano */
void MainWindow::on_actionListen_current_audio_triggered()
{
    if(this->player->state() == QMediaPlayer::PlayingState || this->player->mediaStatus() == QMediaPlayer::EndOfMedia){
        this->player->stop();
        if(!m_currenttitle->m_touch)
            this->ui->textEdit->setHtml(m_currenttitle->testi);

        this->ui->statusBar->clearMessage();
    }else{
        cambioiconariascolto(this);

        if(!m_currenttitle->m_touch)
            this->ui->textEdit->setEnabled(true);

        this->ui->actionListen_current_audio->setEnabled(true);

        this->player->play();
    }
    return;
    this->contrUi();
}
