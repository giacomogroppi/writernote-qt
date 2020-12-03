#ifndef ENABLE_RIASCOLTO_CPP
#define ENABLE_RIASCOLTO_CPP

#include "../mainwindow.h"
#include "ui_mainwindow.h"
#include <QDebug>
#include "../dialog_critic.h"

#include "../setting_ui.h"


#include "cambioiconariascolto.h"

/* la funzione gestisce la gestione del click del punsante con la mano */
void MainWindow::on_actionListen_current_audio_triggered()
{
    cambioiconariascolto(this);
    if(this->player->state() == QMediaPlayer::PlayingState){

        settingaudio_riascoltoinatto(this, false);

        /* ribilitazione del pulsanti per lo stile, e per modifica su testo [copia, incolla...] */
        settingstyle(this, true);
        settingtextedit(this, true);

        this->ui->textEdit->setHtml(this->self->currenttitle.testi);
        this->player->stop();

        this->ui->actionListen_current_audio->setChecked(false);

        return;
    }

    settingaudio_riascoltoinatto(this, true);
    settingstyle(this, false);
    settingtextedit(this, false);
    this->ui->textEdit->setEnabled(true);

    this->ui->actionListen_current_audio->setEnabled(true);

    if(this->self->currenttitle.versione > 0)
        return dialog_critic("You created this file with a too recent version of writernote.\nUpdate it to be able to play this audio");

    this->player->play();
    //this->ui->actionListen_current_audio->setChecked(true);
    this->ui->stopriascolto->setEnabled(true);
}

#endif //ENABLE_RIASCOLTO_CPP
