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
#ifdef STAMPA
    qDebug() << "Inizio lo spacchettamento";
#endif

    messaggio_utente("I begin to sort out the data.\nThis operation may take a while");

#ifdef STAMPA
    qDebug() << "\n\nsavefile::savefile_check_file testinohtml: -> " << this->self->currenttitle.testinohtml.length();
    qDebug() << "\n\nsavefile::savefile_check_file posizione_iniz: -> " << this->self->currenttitle.posizione_iniz.length();
#endif


    spacchettamento(this);

    int i;
    for(i=0; i<this->self->currenttitle.testinohtml.length(); i++)
        qDebug() << this->self->currenttitle.testinohtml[i];
}

#endif // STOP_RECORDING_CPP
