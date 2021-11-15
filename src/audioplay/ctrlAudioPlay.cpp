#include "../mainwindow.h"
#include "ui_mainwindow.h"
#include "audioplay.h"
#include <QDebug>
#include "../utils/dialog_critic/dialog_critic.h"

void MainWindow::on_volumeSlider_actionTriggered(int action)
{
    qDebug() <<"MainWindow::on_volumeSlider_actionTriggered "<< action;
    this->m_audioplayer->setVolume(action);
}

void MainWindow::on_audioSlider_valueChanged(int value)
{
    this->m_audioplayer->setPositionSecond(value);
}

void MainWindow::on_volumeSlider_valueChanged(int value)
{
    qDebug() <<"MainWindow::on_volumeSlider_valueChanged "<< value;
    this->m_audioplayer->setVolume(value);
}

void MainWindow::on_start_play_clicked()
{
    this->m_audioplayer->changeIcon();
    if(this->m_audioplayer->isPlay())
        m_audioplayer->pause();
    else
        m_audioplayer->play();

}

/* gestione del click del punsante con la mano */
void MainWindow::on_actionListen_current_audio_triggered()
{
    if(this->m_audioplayer->isPlay() || this->m_audioplayer->isEndMedia()){
        m_audioplayer->stop();

        this->ui->statusBar->clearMessage();
    }else{
        this->m_audioplayer->changeIcon();

        this->ui->actionListen_current_audio->setEnabled(true);

        m_audioplayer->play();
    }
    this->contrUi();
}

void MainWindow::on_stop_play_clicked()
{
    this->on_actionListen_current_audio_triggered();
}
