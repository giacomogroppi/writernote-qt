#include "../mainwindow.h"
#include "ui_mainwindow.h"

#include <QMessageBox>
#include "../utils/dialog_critic/dialog_critic.h"
#include "../windows/mostra_finestra_i.h"

#include "../audiosetting/loadqualita.h"
#include "../utils/setting_define.h"
#include "../datawrite/qfilechoose.h"
#include "../utils/default_location/audio_default_location.h"

#include "../utils/path/get_path.h"

#include <QSettings>

#ifdef SNAP
static void save();
static bool load();
#endif //snap

/* funzione che gestisce lo start della registrazione */
void MainWindow::on_start_rec_triggered()
{
    if(this->m_currenttitle->se_registato != Document::not_record)
        return user_message("You had already record and audio");
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

    if(!this->setOutputLocation()){
        m_currenttitle->se_registato = Document::not_record;
        return;
    }


    if (this->m_audioRecorder->state() == QMediaRecorder::StoppedState) {
        loadqualita(this);

        /*
         * we reserve 32 MB for the buffer of the audio
        */
        //this->m_currenttitle->audio_data.reserve(32*1024*1024);

        this->m_audioRecorder->record();

        this->m_currenttitle->testinohtml.clear();
        this->m_currenttitle->posizione_iniz.clear();
    }

    this->ui->start_rec->setEnabled(false);
    this->ui->stop_rec->setEnabled(true);
    this->ui->pause_rec->setEnabled(true);

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

    if(audio_default_location::load_default() != audio_default_location::not_define)
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
    if(this->m_currenttitle->se_registato == Document::record_file){
        QString fileName;
        if(!qfilechoose::filechoose(fileName, TYPEAUDIO)){
            return false;
        }

        m_currenttitle->audio_position_path = fileName;
        this->m_audioRecorder->setOutputLocation(QUrl::fromLocalFile(fileName));
        this->m_outputLocationSet = true;
        goto ok;

    }
    else if(this->m_currenttitle->se_registato == Document::record_zip){
        const QString path = get_path(path::audio_pos);
        if(path == ""){
            dialog_critic("I had an internal problem with the audio");
            return false;
        }

        /*
         * what it does is go to create a temporary file to save the audio.
         * we can't save it in a qbytearray
        */
        QString temp_p = path;
        temp_p += POS_AUDIO(m_currenttitle);

        /* testing */
        this->m_audioRecorder->setOutputLocation(QUrl::fromLocalFile(temp_p));
        goto ok;
    }

    free_:
    m_currenttitle->se_registato = Document::not_record;
    DELETE_MENU(menu);
    return false;

    ok:
    DELETE_MENU(menu);
    return true;
}
