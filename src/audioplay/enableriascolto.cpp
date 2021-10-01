#include "../mainwindow.h"
#include "ui_mainwindow.h"
#include "../utils/dialog_critic/dialog_critic.h"
#include "../setting_ui.h"
#include "cambioiconariascolto.h"

/* gestione del click del punsante con la mano */
void MainWindow::on_actionListen_current_audio_triggered()
{
    if(this->player->state() == QMediaPlayer::PlayingState || this->player->mediaStatus() == QMediaPlayer::EndOfMedia){
        this->player->stop();
        this->m_canvas->riascolto(0);

        this->ui->statusBar->clearMessage();
    }else{
        cambioiconariascolto(this);

        this->ui->actionListen_current_audio->setEnabled(true);

        this->player->play();
    }
    this->contrUi();
}
