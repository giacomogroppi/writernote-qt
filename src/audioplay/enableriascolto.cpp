#ifndef ENABLE_RIASCOLTO_CPP
#define ENABLE_RIASCOLTO_CPP

#include "../mainwindow.h"
#include "ui_mainwindow.h"
#include <QDebug>
#include "../dialog_critic.h"

/* la funzione gestisce la gestione del click del punsante con la mano */
void MainWindow::on_actionListen_current_audio_triggered()
{
    if(this->player->state() == QMediaPlayer::PlayingState){
        this->ui->textEdit->setEnabled(true);
        this->ui->actionDelete_audio->setEnabled(true);
        this->ui->actionDelete_copybook->setEnabled(true);
        this->ui->actionSave_As->setEnabled(true);
        this->ui->actionBug_report->setEnabled(true);
        this->ui->actionNew_File->setEnabled(true);

        this->ui->actionListen_current_audio->setChecked(false);
        this->ui->stopriascolto->setEnabled(false);
        this->player->stop();
        return;
    }

    if(this->self->currenttitle.versione > 0)
        return dialog_critic("You created this file with a too recent version of writernote.\nUpdate it to be able to play this audio");

    this->player->play();
    this->ui->actionListen_current_audio->setChecked(true);
    this->ui->stopriascolto->setEnabled(true);
}

#endif //ENABLE_RIASCOLTO_CPP
