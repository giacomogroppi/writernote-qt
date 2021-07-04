#include "../mainwindow.h"
#include "ui_mainwindow.h"
#include <QMediaPlayer>

#include "cambioiconariascolto.h"

void MainWindow::cambiostatoplayer(QMediaPlayer::State state){
    const bool s = state == QMediaPlayer::StoppedState;

    this->ui->stop_play->setHidden(!s);

    this->ui->volumeSlider->setHidden(!s);
    this->ui->audioSlider->setHidden(!s);

    if(s){
        ui->volumeSlider->setValue(0);
        ui->audioSlider->setValue(0);
    }

    ui->listWidgetSX->setEnabled(s);
    this->ui->insertimagebotton->setEnabled(s);

    this->ui->actionImport_Video->setEnabled(s);
    this->ui->actionDelete_audio->setEnabled(s);
    this->ui->actionDelete_copybook->setEnabled(s);

    /* creazione del nuovo copybook */
    this->ui->actionCreate_new_copybook->setEnabled(s);
    this->ui->actionListen_current_audio->setChecked(!s);
    /* cambiare il font in grandezza e in tipo*/
    this->ui->spinBox_fontsize->setEnabled(s);
    this->ui->fontComboBox_fonttipo->setEnabled(s);
    this->ui->inserttablebotton->setEnabled(s);
    this->ui->listbotton->setEnabled(s);
    this->ui->boldbotton->setEnabled(s);

    /* l'utente ha deciso di stoppare l'audio oppure è finito */
    if(state == QMediaPlayer::StoppedState){

        this->ui->start_rec->setEnabled(false);

        QIcon icon;
        icon.addFile(QString::fromUtf8(":image/images/play.png"), QSize(), QIcon::Normal, QIcon::Off);
        this->ui->start_play->setIcon(icon);

        this->ui->statusBar->clearMessage();

        this->ui->textEdit->setHtml(this->m_currenttitle->testi);

        m_canvas->m_pos_ris = -1;
    }

    this->ui->start_rec->setEnabled(false);
}
