#include "deleteaudio.h"

#include "../mainwindow.h"
#include "../utils/areyousure/areyousure.h"
#include "ui_mainwindow.h"

#include "../audioplay/aggiornotastiriascolto.h"

#define CURRENTTITLE parent->m_currenttitle

void deleteaudio(MainWindow *parent){
    if(!areyousure("", "Are you sure you want to delete the audio?"))
        return;

    CURRENTTITLE->testinohtml.clear();
    CURRENTTITLE->posizione_iniz.clear();
    CURRENTTITLE->se_registato = Document::not_record;

    /*
     * TODO -> remove the file on the zip
    */

    CURRENTTITLE->audio_position_path = "";

    aggiornotestiriascolto(parent);
}

void MainWindow::on_actionDelete_audio_triggered()
{
    deleteaudio(this);
}
