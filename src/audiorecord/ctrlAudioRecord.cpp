#include "../mainwindow.h"
#include "ui_mainwindow.h"

/* funzione che gestisce l'aggiornamento della status bar in base al valore del tempo dell'audio */
void MainWindow::updateProgress(qint64 duration)
{
    /* ogni volta che si aggiorna il tempo cambia la variabile nella classe in TabletCanvas -> quando viene scritto qualcosa lo inserisce */
    m_canvas->time = duration;

    if (m_audioRecorder->error() != QMediaRecorder::NoError)
        return;

    this->ui->statusBar->showMessage(tr("Recorded %1 sec").arg(duration / 1000));
}
