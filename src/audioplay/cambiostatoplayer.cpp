#include "../mainwindow.h"
#include "ui_mainwindow.h"
#include <QMediaPlayer>

#include "cambioiconariascolto.h"

void MainWindow::cambiostatoplayer(QMediaPlayer::State state){
    contrUi();/*
    const bool s = (state == QMediaPlayer::StoppedState);

    ui->stop_play->setHidden(s);
    ui->start_play->setHidden(s);

    ui->volumeSlider->setHidden(s);
    ui->audioSlider->setHidden(s);

    if(s){
        ui->volumeSlider->setValue(0);
        ui->audioSlider->setValue(0);
    }

    ui->listWidgetSX->setEnabled(s);
    ui->insertimagebotton->setEnabled(s);

    ui->actionImport_Video->setEnabled(s);
    ui->actionDelete_audio->setEnabled(s);
    ui->actionDelete_copybook->setEnabled(s);

    ui->actionCreate_new_copybook->setEnabled(s);
    ui->actionListen_current_audio->setChecked(!s);

    ui->spinBox_fontsize->setEnabled(s);
    ui->fontComboBox_fonttipo->setEnabled(s);
    ui->inserttablebotton->setEnabled(s);
    ui->listbotton->setEnabled(s);
    ui->boldbotton->setEnabled(s);*/

    /* l'utente ha deciso di stoppare l'audio oppure è finito */
    if(state == QMediaPlayer::StoppedState){

        ui->start_rec->setEnabled(false);

        QIcon icon;
        icon.addFile(QString::fromUtf8(":image/images/play.png"), QSize(), QIcon::Normal, QIcon::Off);
        ui->start_play->setIcon(icon);

        ui->statusBar->clearMessage();

        ui->textEdit->setHtml(this->m_currenttitle->testi);

        m_canvas->m_pos_ris = -1;
    }

    ui->start_rec->setEnabled(false);
}
