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


void MainWindow::on_stoprecordingbotton_triggered()
{
    this->m_audioRecorder->stop();

    /* if we are in keyboard mode */
    if(!m_currenttitle.m_touch){
        progress_bar_ui * m_bar = new progress_bar_ui;
        spacchettamento * m_spac = new spacchettamento(this);

        QObject::connect(m_spac, &spacchettamento::progress, m_bar, &progress_bar_ui::progress_);
        QObject::connect(m_spac, &spacchettamento::finished, m_bar, &progress_bar_ui::finished_);

        QFuture<void> future1 = QtConcurrent::run( m_spac, &spacchettamento::esecuzione );

        m_bar->exec();

        delete m_spac;
        delete m_bar;

    }

    //m_currenttitle.se_registato = true;

    settingaudio_registrazione(this, false);
    settingaudio_riascolto(this, true);

    aggiornotestiriascolto(this);

    this->ui->statusBar->clearMessage();
    this->m_canvas->time = 0;
}
