#ifndef DELETE_AUDIO_CPP
#define DELETE_AUDIO_CPP

#include "deleteaudio.h"

#include "../mainwindow.h"
#include "../utils/areyousure/areyousure.h"
#include "ui_mainwindow.h"

#include "../audioplay/aggiornotastiriascolto.h"

void deleteaudio(MainWindow *parent, QListWidgetItem *item){
    if(!areyousure(parent, "", "Are you sure you want to delete the copybook?"))
        return;


    if(item->text() != parent->self->currentTitle){
        /* we need to load the other file */
        parent->on_listWidgetSX_itemDoubleClicked(item);
        //parent->on_listWidgetSX_itemClicked(item);

        if(item->text() != parent->self->currentTitle)
            return;
    }

    parent->self->currenttitle.testinohtml.clear();
    parent->self->currenttitle.posizione_iniz.clear();
    parent->self->currenttitle.se_registato = false;
    parent->self->currenttitle.audio_position_path = "";

    aggiornotestiriascolto(parent);
}

/* funzione di mainwindow */
void MainWindow::on_actionDelete_audio_triggered()
{
    deleteaudio(this, this->ui->listWidgetSX->currentItem());
}

#endif
