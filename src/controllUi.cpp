#include "mainwindow.h"
#include "ui_mainwindow.h"

void MainWindow::contrUi(){
    const bool already_rec =    m_currenttitle->se_registato != Document::not_record;

    const bool is_rec =         m_audioRecorder->state() == QAudioRecorder::RecordingState;
    const bool is_pause_rec =       m_audioRecorder->state() == QAudioRecorder::PausedState;

    const bool is_touch =       m_currenttitle->m_touch;

    const bool is_play =        player->state() == QMediaPlayer::PlayingState;

    ui->pause_rec->setEnabled(is_rec);
    ui->start_rec->setEnabled(!already_rec && !is_pause_rec && !is_rec);
    ui->stop_rec->setEnabled(is_rec);

    m_canvas->setHidden(!is_touch);
    ui->toolSheet->setHidden(!is_touch);
    ui->toolBarcolore->setHidden(!is_touch);
    ui->toolbarmatita->setHidden(!is_touch);

    ui->textEdit->setHidden(is_touch);
    ui->boldbotton->setHidden(is_touch);
    ui->inserttablebotton->setHidden(is_touch);
    ui->fontComboBox_fonttipo->setHidden(is_touch);
    ui->insertimagebotton->setHidden(is_touch);
    ui->spinBox_fontsize->setHidden(is_touch);
    ui->listbotton->setHidden(is_touch);

    ui->start_play->setHidden(!is_play);
    ui->stop_play->setHidden(!is_play);
    ui->audioSlider->setHidden(!is_play);
    ui->volumeSlider->setHidden(!is_play);
    ui->actionDelete_audio->setEnabled(already_rec);

    ui->actionListen_current_audio->setCheckable(true);
    ui->actionListen_current_audio->setEnabled(already_rec);
    ui->actionListen_current_audio->setChecked(is_play);
}
