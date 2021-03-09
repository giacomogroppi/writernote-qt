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
#include <QMessageBox>
#include "../utils/path/get_path.h"

static void removeAudio(currenttitle_class *);
static bool saveAudio(currenttitle_class * , QString &path);

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
        QObject::connect(m_bar, &progress_bar_ui::closeForce, m_spac, &spacchettamento::exit);

        QFuture<void> future1 = QtConcurrent::run( m_spac, &spacchettamento::esecuzione );

        m_bar->exec();

        delete m_spac;
        delete m_bar;

    }

    if(m_currenttitle->se_registato == audio_record::record_zip){
        if(!saveAudio(m_currenttitle, m_path))
            removeAudio(m_currenttitle);

    }

    settingaudio_registrazione(this, false);
    settingaudio_riascolto(this, true);

    aggiornotestiriascolto(this);

    this->ui->statusBar->clearMessage();
    this->m_canvas->time = 0;
}

static bool saveAudio(currenttitle_class *m_currenttitle, QString &m_path){
    const char * path = get_path_no_controll();
    if(!path){
        if(!QFile::exists(path))
            dialog_critic("For some reason the audio file you just recorded no longer exists\n, if you moved it, reposition it where you got it, with the same name");


    }

    if(save_audio_file(m_currenttitle->audio_data, m_currenttitle->nome_copybook, m_path) != OK)
        dialog_critic("We had a problem saving the audio into " + m_path);

    savefile m_save(&m_path, m_currenttitle);
    if(m_save.savefile_check_file() != OK)
        dialog_critic("We had a problem saving the current copybook");

    return true;
}

static void removeAudio(currenttitle_class *data){
    data->se_registato = audio_record::not_record;
    data->audio_data.clear();
}
