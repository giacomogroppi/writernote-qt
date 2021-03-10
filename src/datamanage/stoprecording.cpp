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
#include "../utils/retry/retry_ui.h"

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
            /* TODO -> reset button */;

    }

    settingaudio_registrazione(this, false);
    settingaudio_riascolto(this, true);

    aggiornotestiriascolto(this);

    this->ui->statusBar->clearMessage();
    this->m_canvas->time = 0;
}


/*
 * to connect two object we need a qobject
*/
class reciver: public QObject{
    Q_OBJECT
private:
    currenttitle_class *m_current;
    bool *m_exist;
public:
    reciver(currenttitle_class *data, bool *m_exist){
        this->m_current = data;
        this->m_exist = m_exist;
    }

    bool exist();
public slots:
    void retry();
    void close();

signals:
    void resultRetry(bool);
};

static bool saveAudio(currenttitle_class *m_currenttitle, QString &m_path){
    const char * path = get_path_no_controll();

    if(!QFile::exists(path)){
        bool save = false;

        reciver m_reciver(m_currenttitle, &save);
        retry_ui m_r(nullptr, "Audio missing", "For some reason the audio file you just recorded no longer exists\n, if you moved it, reposition it where you got it, with the same name", "The file does not exist");

        QObject::connect(&m_r, &retry_ui::retry, &m_reciver, &reciver::retry);
        QObject::connect(&m_r, &retry_ui::close, &m_reciver, &reciver::close);

        QObject::connect(&m_reciver, &reciver::resultRetry, &m_r, &retry_ui::resultRetry);

        m_r.exec();

        if(!save)
            return false;
    }

    if(save_audio_file(m_currenttitle->audio_data, m_currenttitle->nome_copybook, m_path) != OK)
        dialog_critic("We had a problem saving the audio into " + m_path);

    if(savefile(&m_path, m_currenttitle).savefile_check_file() != OK)
        dialog_critic("We had a problem saving the current copybook");

    return true;
}

static void removeAudio(currenttitle_class *data){
    data->se_registato = audio_record::not_record;
    data->audio_data.clear();
}

/*
 * return true if the audio file exist
*/
bool reciver::exist(){
    QString path = get_path_no_controll();
    return QFile::exists(path + this->m_current->nome_copybook + POS_TEMP_AUDIO);
}

void reciver::retry(){
    *this->m_exist = true;
    emit resultRetry(this->exist());
}

/*
 * this function is call if the user
 * close the window
*/
void reciver::close(){
    this->m_current->posizione_iniz.clear();
    this->m_current->testinohtml.clear();
}
