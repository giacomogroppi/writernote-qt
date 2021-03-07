#include "../mainwindow.h"
#include "ui_mainwindow.h"

#include <QString>

#include "../utils/dialog_critic/dialog_critic.h"

#include "../audioplay/aggiornotastiriascolto.h"

#include "../setting_ui.h"
#include "spacchettamento.h"

#include "../utils/progress_bar/progress_bar_ui.h"
#include <QThread>
#include <QFuture>
#include <QtConcurrent/QtConcurrent>

#include "../datawrite/savefile.h"

void MainWindow::on_stoprecordingbotton_triggered()
{
    if(this->m_audioRecorder->state() == QMediaRecorder::StoppedState)
        return;

    this->m_audioRecorder->stop();

    /* if we are in keyboard mode */
    if(!m_currenttitle->m_touch){
        progress_bar_ui * m_bar = new progress_bar_ui;
        spacchettamento * m_spac = new spacchettamento(this);

        QObject::connect(m_spac, &spacchettamento::progress, m_bar, &progress_bar_ui::progress_);
        QObject::connect(m_spac, &spacchettamento::finished, m_bar, &progress_bar_ui::finished_);

        QFuture<void> future1 = QtConcurrent::run( m_spac, &spacchettamento::esecuzione );

        m_bar->exec();

        delete m_spac;
        delete m_bar;

    }

    if(m_currenttitle->se_registato == audio_record::record_zip){
        if(save_audio_file(m_currenttitle->audio_data, m_currenttitle->nome_copybook, m_path) != OK)
            dialog_critic("We had a problem saving the audio into " + m_path);

        savefile m_save(&m_path, m_currenttitle);
        if(m_save.savefile_check_file() != OK)
            dialog_critic("We had a problem saving the current copybook");

    }

    settingaudio_registrazione(this, false);
    settingaudio_riascolto(this, true);

    aggiornotestiriascolto(this);

    this->ui->statusBar->clearMessage();
    this->m_canvas->time = 0;
}
