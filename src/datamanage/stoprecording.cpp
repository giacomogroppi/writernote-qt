#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "utils/dialog_critic/dialog_critic.h"
#include "audioplay/aggiornotastiriascolto.h"
#include "utils/progress_bar/progress_bar_ui.h"
#include "datawrite/savefile.h"
#include "utils/path/get_path.h"
#include "utils/retry/retry_ui.h"
#include "retry_save_audio.h"
#include "utils/areyousure/areyousure.h"
#include "utils/setting_define.h"
#include "utils/permission/permission.h"
#include "utils/make_default/make_default_ui.h"
#include "audiorecord/audiorecord.h"
#include "utils/time/waitTime.h"

#include <QString>
#include <QSettings>
#include <QMessageBox>
#include <QThread>
#include <QFuture>
#include <QtConcurrent/QtConcurrent>

namespace removeAudio {
    enum n_removeAudio: int{
        remove_ok,
        not_remove_ok,
        ask
    };

    static n_removeAudio removeAudioSettingsLoad();
    static void removeAudioSettingsSave(n_removeAudio);
}

static bool manageNotExist(const QString &path, Document *doc)
{
    bool save = false;

    if(unlikely(!QFile::exists(path))){
        retry_save_audio m_reciver(doc, &save);
        retry_ui m_r(nullptr, "Audio missing",
                     "For some reason the audio file you just recorded no longer exists\n, if you moved it, reposition it where you got it, with the same name",
                     "The file does not exist");


        QObject::connect(&m_reciver, &retry_save_audio::resultRetry, &m_r, &retry_ui::resultRetry_reciver);
        QObject::connect(&m_r, &retry_ui::ui_close, &m_reciver, &retry_save_audio::close);
        QObject::connect(&m_r, &retry_ui::ui_retry, &m_reciver, &retry_save_audio::retry);

        m_r.exec();

        if(!save)
            return false;
    }

    return true;
}

static bool needRemove;

static void saveAudio(
    Document            *m_currenttitle, 
    const QString       &m_path, 
    const AudioRecord   *recorder)
{
    const QString &path = recorder->getPath();
    removeAudio::n_removeAudio temp;

    constexpr int max = 10;
    constexpr int millisecond = 500;
    int i, res;

    if(unlikely(!manageNotExist(path, m_currenttitle)))
        return;

    /*
     * Since it is not possible to synchronize
     * this thread with the thread that saves
     * the file to disk, the only way to wait
     * for the writing to finish is to try to
     * save at time intervals.
    */
    for(i = 0; i < max; i++){
        waitTime(millisecond);
        res = save_audio_file(path.toUtf8().constData(), m_path);
        if(res == OK)
            goto out;
    }

out:
    if(res != OK){
        return dialog_critic("We had a problem saving the audio into " + m_path);
    }

    if(savefile(&m_path, m_currenttitle).savefile_check_file(true) != OK)
        dialog_critic("We had a problem saving the current copybook");

    temp = removeAudio::removeAudioSettingsLoad();

    if(temp == removeAudio::ask){
        needRemove = false;

        /* remove audio */
        make_default_ui temp_ui(nullptr,
                                "Remove temp audio",
                                "With writernote, when you record an audio file, it is automatically saved \non your disk, so that you incur less data loss errors, do you want \nto remove? the location of the file is " + path);

        QObject::connect(&temp_ui, &make_default_ui::no, [=](bool var){
            if(var){
                removeAudio::removeAudioSettingsSave(removeAudio::not_remove_ok);
            }
            needRemove = false;
        });

        QObject::connect(&temp_ui, &make_default_ui::yes, [=](bool var){
            if(var){
                removeAudio::removeAudioSettingsSave(removeAudio::remove_ok);
            }
            needRemove = true;
        });

        temp_ui.exec();
    }

    if(temp == removeAudio::remove_ok || needRemove){
        if(!QFile::remove(path)){
            if(!permission::open(path.toUtf8().constData(), permission::writeOnly))
                dialog_critic("I don't have permission to remove file locate in " + path);
            else
                dialog_critic("We had a problem removing audio locate in " + path);
        }
    }

}

void MainWindow::on_stop_rec_triggered()
{
    Document *doc = _canvas->data;

    if(unlikely(m_audio_recorder->isStopped())){
        log_write->write("The audio is not recording", log_ui::possible_bug);
        return;
    }

    m_audio_recorder->stopRecording();
    qDebug() << "Audio record in: " << m_audio_recorder->getPath();

    if(doc->se_registato == Document::record_zip){
        saveAudio(doc, m_path, m_audio_recorder);
    }

    contrUi();

    aggiornotestiriascolto(this);

    this->ui->statusBar->clearMessage();
}

/*
 * this function return true if the user want
*/
static removeAudio::n_removeAudio removeAudio::removeAudioSettingsLoad(){
    QSettings setting(ORGANIZATIONAME, APPLICATION_NAME);
    setting.beginGroup(GROUPNAME_AUDIO_REMOVE_RECORD);

    int temp = setting.value(KEY_AUDIO_REMOVE_RECORD, removeAudio::ask).toInt();

    setting.endGroup();

    auto temp_ = static_cast<removeAudio::n_removeAudio>(temp);

    return temp_;
}


static void removeAudio::removeAudioSettingsSave(removeAudio::n_removeAudio val){
    QSettings setting(ORGANIZATIONAME, APPLICATION_NAME);
    setting.beginGroup(GROUPNAME_AUDIO_REMOVE_RECORD);

    int temp = static_cast<int>(val);

    setting.setValue(KEY_AUDIO_REMOVE_RECORD, temp);

    setting.endGroup();

}
