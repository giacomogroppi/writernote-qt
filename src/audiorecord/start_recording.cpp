#include "../mainwindow.h"
#include "ui_mainwindow.h"

#include <QMessageBox>
#include "../utils/dialog_critic/dialog_critic.h"
#include "../windows/mostra_finestra_i.h"

#include "../audiosetting/loadqualita.h"
#include "../utils/setting_define.h"

#include <QSettings>

#ifdef SNAP
static void save();
static bool load();
#endif //snap

/* funzione che gestisce lo start della registrazione */
void MainWindow::on_startrecording_triggered()
{
    if(this->m_currenttitle.audio_position_path != "")
        return dialog_critic("You had already record an audio");

/* only on snap package we have this proble */
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

    if(this->m_currenttitle.audio_position_path == "")
        return;

    if (this->m_audioRecorder->state() == QMediaRecorder::StoppedState) {
        loadqualita(this);

        this->m_audioRecorder->record();

        this->m_currenttitle.testinohtml.clear();
        this->m_currenttitle.posizione_iniz.clear();
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
