#ifndef STOP_RECORDING_CPP
#define STOP_RECORDING_CPP

#include "../mainwindow.h"
#include "ui_mainwindow.h"

#include <QDebug>
#include <QString>

#include "sistemazionedatiregistrati.cpp"
#include "../dialog_critic.h"

#include "../audioplay/aggiornotastiriascolto.h"

#include "../setting_ui.h"

void MainWindow::on_stoprecordingbotton_triggered()
{
    this->m_audioRecorder->stop();

    messaggio_utente("I begin to sort out the data.\nThis operation may take a while");

    spacchettamento(this);
    this->self->currenttitle.se_registato = true;

    messaggio_utente("I finished");

    settingaudio_registrazione(this, false);
    settingaudio_riascolto(this, true);

    aggiornotestiriascolto(this);

    //this->ui->statusBar->showMessage(tr("Audio save in %1").arg(this->self->currenttitle.audio_position_path));
    this->ui->statusBar->clearMessage();
}


#endif // STOP_RECORDING_CPP
