#include "../mainwindow.h"
#include "ui_mainwindow.h"

/* funzione che gestisce l'aggiornamento della status bar in base al valore del tempo dell'audio */
void MainWindow::updateProgress(qint64 duration)
{
    /* ogni volta che si aggiorna il tempo cambia la variabile nella classe in TabletCanvas -> quando viene scritto qualcosa lo inserisce */
    if(this->m_currenttitle.m_touch){
        m_canvas->time = duration;
    }

    //this->currentTime = (int)(duration/1000);
    if (m_audioRecorder->error() != QMediaRecorder::NoError)
        return;

    this->ui->statusBar->showMessage(tr("Recorded %1 sec").arg(duration / 1000));
}
