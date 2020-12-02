#include "../mainwindow.h"
#include "ui_mainwindow.h"
#include <QMediaPlayer>

void MainWindow::cambiostatoplayer(QMediaPlayer::State state){
    /* l'utente ha deciso di stoppare l'audio oppure è finito */
    if(state == QMediaPlayer::StoppedState){
        this->ui->audioSlider->setValue(0);
        this->ui->audioSlider->setEnabled(false);

        this->ui->volumeSlider->setValue(0);
        this->ui->volumeSlider->setEnabled(false);
        this->ui->stopriascolto->setEnabled(false);
        this->ui->startrecording->setEnabled(false);

        /* pulsante della mano -> se si clicca viene disattivato il riascolto */
        this->ui->actionListen_current_audio->setChecked(true);


        this->ui->actionImport_Video->setEnabled(true);
        this->ui->actionDelete_audio->setEnabled(true);
        this->ui->actionDelete_copybook->setEnabled(true);

        this->ui->listWidgetSX->setEnabled(true);

        /* creazione del nuovo copybook */
        this->ui->actionCreate_new_copybook->setEnabled(true);

        /* cambiare il font in grandezza e in tipo*/
        this->ui->spinBox_fontsize->setEnabled(true);
        this->ui->fontComboBox_fonttipo->setEnabled(true);
        this->ui->insertimagebotton->setEnabled(true);
        this->ui->inserttablebotton->setEnabled(true);
        this->ui->listbotton->setEnabled(true);


        this->ui->textEdit->setHtml(this->self->currenttitle.testi);

    }
    else if(state == QMediaPlayer::PlayingState || state == QMediaPlayer::PausedState){
        this->ui->listWidgetSX->setEnabled(false);

        this->ui->audioSlider->setEnabled(true);

        this->ui->volumeSlider->setEnabled(false);
        this->ui->stopriascolto->setEnabled(true);
        this->ui->startrecording->setEnabled(false);

        /* pulsante della mano -> se si clicca viene disattivato il riascolto */
        this->ui->actionListen_current_audio->setChecked(true);

        this->ui->actionImport_Video->setEnabled(false);
        this->ui->actionDelete_audio->setEnabled(false);
        this->ui->actionDelete_copybook->setEnabled(false);

        this->ui->listWidgetSX->setEnabled(true);

        this->ui->actionCreate_new_copybook->setEnabled(false);
        this->ui->actionConvert_audio_to_text->setEnabled(false);

        this->ui->spinBox_fontsize->setEnabled(false);
        this->ui->fontComboBox_fonttipo->setEnabled(false);

        this->ui->insertimagebotton->setEnabled(false);
        this->ui->inserttablebotton->setEnabled(false);
        this->ui->listbotton->setEnabled(false);
        this->ui->boldbotton->setEnabled(false);
    }

}
