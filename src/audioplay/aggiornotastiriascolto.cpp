#ifndef AGGIORNO_TASTI_RIASCOLTO_CPP
#define AGGIORNO_TASTI_RIASCOLTO_CPP

#include "aggiornotastiriascolto.h"

#include "../mainwindow.h"
#include "ui_mainwindow.h"
#include <QFile>
#include <QDebug>

#include "../dialog_critic.h"
#include "../setting_ui.h"

void aggiornotestiriascolto(MainWindow *parent){
    if(parent->self->currenttitle.se_registato){
        QFile file(parent->self->currenttitle.audio_position_path.c_str());
        if(!file.exists())
            return dialog_critic("Audio file didn't exist");

        settingaudio_registrazione(parent, false);

        /* enable play, pause botton for play audio alreay record */
        parent->ui->stopriascolto->setEnabled(true);
        parent->ui->startriascoltobotton->setEnabled(true);

        /* volume and audio slider */
        parent->ui->audioSlider->setEnabled(true);
        parent->ui->volumeSlider->setEnabled(true);

        /* abilita il bottone della mano e lo segna unchecked, in caso sia cliccato per qualche motivo*/
        parent->ui->actionListen_current_audio->setEnabled(true);
        parent->ui->actionListen_current_audio->setChecked(false);

        parent->player->setMedia(QUrl::fromLocalFile(parent->self->currenttitle.audio_position_path.c_str()));
    }
    else{
        /* abilita i tasti per la registrazione */
        settingaudio_registrazione(parent, true);
        settingaudio_riascolto(parent, false);

        parent->ui->actionListen_current_audio->setChecked(false);
        parent->ui->actionListen_current_audio->setEnabled(false);
    }

    if(parent->self->currenttitle.se_tradotto){

    }
}


#endif
