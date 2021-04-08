#include "deleteaudio.h"

#include "../mainwindow.h"
#include "../utils/areyousure/areyousure.h"
#include "ui_mainwindow.h"

#include "../audioplay/aggiornotastiriascolto.h"

#define CURRENTTITLE parent->m_currenttitle

void deleteaudio(MainWindow *parent, QListWidgetItem *item){
    if(!areyousure(parent, "", "Are you sure you want to delete the copybook?"))
        return;


    if(item->text() != parent->m_currentTitle){
        /* we need to load the other file */
        parent->on_listWidgetSX_itemDoubleClicked(item);
        //parent->on_listWidgetSX_itemClicked(item);

        if(item->text() != parent->m_currentTitle)
            return;
    }

    CURRENTTITLE->testinohtml.clear();
    CURRENTTITLE->posizione_iniz.clear();
    CURRENTTITLE->se_registato = currenttitle_class::not_record;

    /*
     * TODO -> remove the file on the zip
    */

    CURRENTTITLE->audio_position_path = "";

    aggiornotestiriascolto(parent);
}

void MainWindow::on_actionDelete_audio_triggered()
{
    deleteaudio(this, this->ui->listWidgetSX->currentItem());
}
