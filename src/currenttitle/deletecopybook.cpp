#ifndef DELETE_COPY_BOOK_CPP
#define DELETE_COPY_BOOK_CPP

#include "../mainwindow.h"
#include "ui_mainwindow.h"
#include "../areyousure.h"
#include <QMessageBox>
#include <QString>

#include "../update_list_copybook.h"

#include <QMediaPlayer>
#include "redolist.cpp"

/* funzione che gestisce l'eliminizione del copybook */
void f_deletecopybook( MainWindow *parent, const char *copybook = nullptr){
    if(!areyousure(parent, "Delete copybook", "are you sure you want to delete?"))
        return redolist(parent);

    if(parent->player->state() != QMediaPlayer::StoppedState)
        parent->player->stop();

    int posizione = parent->self->indice.titolo.indexOf(copybook);

    parent->self->indice.titolo.removeAt(posizione);
    parent->self->indice.audio.removeAt(posizione);
    parent->self->indice.compressione.removeAt(posizione);
    parent->self->indice.video.removeAt(posizione);
    //parent->self->indice.file_testo.removeAt(posizione);

    parent->self->currenttitle.reset();

    parent->ui->textEdit->setEnabled(false);
    parent->ui->actionDelete_audio->setEnabled(false);
    parent->ui->actionDelete_copybook->setEnabled(false);

    parent->ui->stopriascolto->setEnabled(false);
    parent->ui->startriascoltobotton->setEnabled(false);

    parent->ui->startrecording->setEnabled(false);
    parent->ui->startriascoltobotton->setEnabled(false);

    parent->ui->textEdit->setHtml("");

}

void MainWindow::on_actionDelete_copybook_triggered()
{
    if (this->self->currentTitle == "")
        return;

    f_deletecopybook(this, this->self->currentTitle.c_str());
    this->self->currentTitle = "";
    update_list_copybook(this);

    this->self->indice.stampa();
}

#endif
