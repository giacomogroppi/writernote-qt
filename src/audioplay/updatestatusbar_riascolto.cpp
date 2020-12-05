#include "../mainwindow.h"
#include "ui_mainwindow.h"

/* funzione che gestisce l'aggiornamento della status bar in base al valore del tempo dell'audio */
void MainWindow::updateProgress(qint64 duration)
{
    this->self->currentTime = (int)(duration/1000);
    if (m_audioRecorder->error() != QMediaRecorder::NoError || duration < 2000)
        return;

    this->ui->statusBar->showMessage(tr("Recorded %1 sec").arg(duration / 1000));
}
