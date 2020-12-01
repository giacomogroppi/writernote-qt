#ifndef AGGIORNO_TASTI_RIASCOLTO_CPP
#define AGGIORNO_TASTI_RIASCOLTO_CPP

#include "../mainwindow.h"
#include "ui_mainwindow.h"
#include <QFile>
#include <QDebug>

void aggiornotestiriascolto(MainWindow *parent){
    if(parent->self->currenttitle.se_registato){
        QFile file(parent->self->currenttitle.audio_position_path.c_str());
        if(!file.exists())
            return;

        /* disable play, stop, pause botton for record audio */
        parent->ui->pauserecordingbotton->setEnabled(false);
        parent->ui->startrecording->setEnabled(false);
        parent->ui->stopriascolto->setEnabled(false);

        /* enable play, pause botton for play audio alreay record */
        parent->ui->stopriascolto->setEnabled(true);
        parent->ui->startriascoltobotton->setEnabled(true);

        /* volume and audio slider */
        parent->ui->audioSlider->setEnabled(true);
        parent->ui->volumeSlider->setEnabled(true);

        parent->player->setMedia(QUrl::fromLocalFile(parent->self->currenttitle.audio_position_path.c_str()));
    }

    if(parent->self->currenttitle.se_tradotto){

    }
}


#endif
