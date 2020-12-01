#ifndef STOP_RECORDING_CPP
#define STOP_RECORDING_CPP

#include "../mainwindow.h"
#include "ui_mainwindow.h"
#include <QDebug>
#include "sistemazionedatiregistrati.cpp"
#include "../dialog_critic.h"

/* funzione che gestisce lo stop della registrazione */
void MainWindow::on_stopplaybotton_triggered()
{
    this->m_audioRecorder->stop();

    messaggio_utente("I begin to sort out the data.\nThis operation may take a while");

    spacchettamento(this);
}

#endif // STOP_RECORDING_CPP
