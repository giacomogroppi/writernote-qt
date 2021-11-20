#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "audioplay/audioplay.h"
#include "audiorecord/audiorecord.h"

void MainWindow::contrUi(){
    const bool already_rec =    m_currenttitle->se_registato != Document::not_record;

    const bool is_recording =   m_audio_recorder->isRecording();

    const bool is_pause_rec =   m_audio_recorder->isPauseRecording();
    const bool is_play =        this->m_audioplayer->isPlay();

    ui->pause_rec->setEnabled(is_recording);
    ui->start_rec->setEnabled(!already_rec && !is_pause_rec && !is_recording);
    ui->stop_rec->setEnabled(is_recording);

    ui->start_play->setHidden(!is_play);
    ui->stop_play->setHidden(!is_play);
    ui->audioSlider->setHidden(!is_play);
    ui->volumeSlider->setHidden(!is_play);
    ui->actionDelete_audio->setEnabled(already_rec);

    ui->actionListen_current_audio->setEnabled(already_rec);
    ui->actionListen_current_audio->setChecked(is_play);
}
