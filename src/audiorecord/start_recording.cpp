#include "../mainwindow.h"
#include "ui_mainwindow.h"

#include <QMessageBox>
#include "../utils/dialog_critic/dialog_critic.h"
#include "../windows/mostra_finestra_i.h"

#include "../audiosetting/loadqualita.h"
#include "../utils/setting_define.h"
#include "../datawrite/qfilechoose.h"
#include "../utils/default_location/audio_default_location.h"

#include <QSettings>

#ifdef SNAP
static void save();
static bool load();
#endif //snap

/* funzione che gestisce lo start della registrazione */
void MainWindow::on_startrecording_triggered()
{
    if(this->m_currenttitle->se_registato != audio_record::not_record)
        return messaggio_utente("You had already record and audio");
    /*
    if(this->m_currenttitle.audio_position_path != "")
        return dialog_critic("You had already record an audio");*/

/* only on snap package we have this problem */
#ifdef SNAP
    if(!load()){
        QMessageBox::StandardButton resBtn = QMessageBox::question( nullptr, "Warning",
                                                                    "to record audio I need permissions, type on a snap connect terminal writernote: audio-record\notherwise I will not be able to record audio",
                                                                    QMessageBox::Ok | QMessageBox::Cancel | QMessageBox::Help,
                                                                    QMessageBox::Ok);

        if(resBtn == QMessageBox::Help){
            return mostra_finestra_i("https://github.com/giacomogroppi/writernote-qt/blob/master/help/help-snapcraft-audio.md");
        }
        else if(resBtn == QMessageBox::Ok){
            save();
        }
        else if(resBtn == QMessageBox::Cancel){
            return;
        }
    }
#endif //snap

    if(!this->setOutputLocation())
        return;


    if (this->m_audioRecorder->state() == QMediaRecorder::StoppedState) {
        loadqualita(this);

        /*
         * we reserve 32 MB for the buffer of the audio
        */
        this->m_currenttitle->audio_data.reserve(32*1024*1024);

        this->m_audioRecorder->record();

        this->m_currenttitle->testinohtml.clear();
        this->m_currenttitle->posizione_iniz.clear();
    }

    this->ui->startrecording->setEnabled(false);
    this->ui->stoprecordingbotton->setEnabled(true);
    this->ui->pauserecordingbotton->setEnabled(true);

}

#ifdef SNAP
static bool load(){
    bool ok = false;
    QSettings setting(ORGANIZATIONAME, APPLICATION_NAME);
    setting.beginGroup(GROUPNAME_AUDIO_PERMISSION);

    ok = setting.value(KEY_AUDIO_PERMISSION, false).toBool();

    setting.endGroup();

    return ok;
}

/* it's only save if the user click ok */
static void save(){
    QSettings setting(ORGANIZATIONAME, APPLICATION_NAME);
    setting.beginGroup(GROUPNAME_AUDIO_PERMISSION);

    setting.setValue(KEY_AUDIO_PERMISSION, true);

    setting.endGroup();
}
#endif //snap


#define DELETE_MENU(x) if(x)delete x

/*
 * TODO make an android version of the function
*/
bool MainWindow::setOutputLocation()
{
#ifdef Q_OS_WINRT
    // UWP does not allow to store outside the sandbox
    const QString cacheDir = QStandardPaths::writableLocation(QStandardPaths::CacheLocation);
    if (!QDir().mkpath(cacheDir)) {
        qWarning() << "Failed to create cache directory";
        return;
    }
    QString fileName = cacheDir + QLatin1String("/output.wav");
#else
    //QString fileName = QFileDialog::getSaveFileName();

#endif

    QMenu *menu = nullptr;
    QAction *internal = nullptr, *ext = nullptr;

    QPoint hostRect;

    if(audio_default::load_default() != audio_default::not_define)
        goto procede;

    menu = new QMenu(this);
    menu->setTitle("Chose output location file");

    internal = new QAction(menu); // Assumes actions is not empty
    internal->setStatusTip(tr("Into writernote file [Beta]"));
    internal->setText("Internal file[Beta]");
    menu->addAction(internal);

    ext = new QAction(menu);
    ext->setStatusTip("External file");
    ext->setText("External file");
    menu->addAction(ext);

    connect(internal, &QAction::triggered, this, &MainWindow::setInZipAudio);
    connect(ext, &QAction::triggered, this, &MainWindow::setExtAudio);

    hostRect = this->cursor().pos();
    menu->move(hostRect);

    if(!menu->exec())
        goto free_;


    procede:
    if(this->m_currenttitle->se_registato == audio_record::record_file){
        auto *qfile = new qfilechoose(this);
        QString fileName;
        if(!qfile->filechoose(&fileName, TYPEAUDIO)){
            delete qfile;
            return false;
        }
        delete qfile;

        m_currenttitle->audio_position_path = fileName;
        this->m_audioRecorder->setOutputLocation(QUrl::fromLocalFile(fileName));
        this->m_outputLocationSet = true;
        goto ok;

    }
    else if(this->m_currenttitle->se_registato == audio_record::record_zip){
        this->m_audioRecorder->setOutputLocation(QUrl::fromAce(this->m_currenttitle->audio_data));
        goto ok;
    }

    free_:
    m_currenttitle->se_registato = audio_record::not_record;
    DELETE_MENU(menu);
    return false;

    ok:
    DELETE_MENU(menu);
    return true;
}

#include <QDebug>

/*
 * this function append the data from the audio record buffer
 * to the buffer on the currenttitle
*/
void MainWindow::progressBuffer(const QAudioBuffer &buffer){
    /*
     * only if we are saving the audio into a writernote file
     * we need to allocate it into a buffer
    */
    if(m_currenttitle->se_registato == audio_record::record_zip){
        this->m_currenttitle->audio_data.append(buffer.constData<char>(), buffer.byteCount());
    }
}
