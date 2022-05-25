#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QMessageBox>
#include "utils/dialog_critic/dialog_critic.h"
#include "windows/mostra_finestra_i.h"
#include "utils/setting_define.h"
#include "datawrite/qfilechoose.h"
#include "utils/default_location/audio_default_location.h"
#include "utils/path/get_path.h"
#include "utils/get_path_application.h"
#include "audiorecord.h"
#include "utils/slash/slash.h"
#include <QSettings>
#include <QDir>

#ifdef SNAP
static void save();
static bool load();
#endif //snap

/* funzione che gestisce lo start della registrazione */
void MainWindow::on_start_rec_triggered()
{
    Document *doc = _canvas->data;
    const QPoint & pointAction = cursor().pos();

    if(doc->se_registato != Document::not_record)
        return user_message(QApplication::tr("You had already record and audio"));

/* only on snap package we have this problem */
#ifdef SNAP
    if(!load()){
        QMessageBox::StandardButton resBtn = QMessageBox::question( nullptr, QApplication::tr("Warning"),
                                                                    QApplication::tr("to record audio I need permissions, type on a snap connect terminal writernote: audio-record\notherwise I will not be able to record audio"),
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

    if(!this->setOutputLocation(pointAction)){
        doc->se_registato = Document::not_record;
        return;
    }

    if (m_audio_recorder->isStopped()) {
        this->m_audio_recorder->loadSettings();

        m_audio_recorder->startRecord();
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

/*
 * TODO make an android version of the function
*/
bool MainWindow::setOutputLocation(const QPoint &hostRect)
{
    QString tmp_internal;
    Document *doc = _canvas->data;

#if defined (Q_OS_WINRT) || defined (ANDROID_WRITERNOTE) || defined(IOS_WRITERNOTE)
    Q_UNUSED(hostRect);
    const QString &writableDir = get_path_application::exe();
    // UWP does not allow to store outside the sandbox

    if (!QDir().mkpath(writableDir)) {
        qWarning() << "Failed to create cache directory";
        return false;
    }

    tmp_internal = writableDir + QLatin1String( "/output.wav");
    doc->se_registato = Document::n_audio_record::record_zip;
#else
    /* On android and ios the only way to record audio is in a zip file. */
    QAction *internal = nullptr, *ext = nullptr;
    QMenu menu;

    if(audio_default_location::load_default() != audio_default_location::not_define)
        goto procede;


    menu.setTitle(tr("Chose output location file"));

    internal = new QAction(&menu); // Assumes actions is not empty
    internal->setStatusTip(tr("Into writernote file"));
    internal->setText(QApplication::tr("Internal file"));
    menu.addAction(internal);

    ext = new QAction(&menu);
    ext->setStatusTip(QApplication::tr("External file"));
    ext->setText("External file");
    menu.addAction(ext);

    QObject::connect(internal, &QAction::triggered, this, [&]{
         doc->se_registato = Document::n_audio_record::record_zip;
    });
    QObject::connect(ext, &QAction::triggered, this, [&]{
        doc->se_registato = Document::n_audio_record::record_file;
    });

    menu.move(hostRect);

    if(!menu.exec())
        goto free_;

    procede:
    if(doc->se_registato == Document::record_file){
        QString fileName;
        if(!qfilechoose::getFileForSave(fileName, TYPEAUDIO)){
            return false;
        }

        doc->audio_position_path = fileName;
        m_audio_recorder->setOutputLocation(fileName);
        this->_outputLocationSet = true;
        return true;

    }
#endif

    if(doc->se_registato == Document::record_zip){
#if !(defined (ANDROID_WRITERNOTE) || defined(IOS_WRITERNOTE))
        const QString path = get_path(path::audio_pos);
        if(path == ""){
            dialog_critic(QApplication::tr("I had an internal problem with the audio"));
            return false;
        }

        /*
         * what it does is go to create a temporary file to save the audio.
         * we can't save it in a qbytearray
        */
        tmp_internal = path + slash::__slash();
        tmp_internal += NAME_AUDIO;
#endif

        /* testing */
        this->m_audio_recorder->setOutputLocation(tmp_internal);
        return true;
    }

    free_:
    doc->se_registato = Document::not_record;
    return false;
}
