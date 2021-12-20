#include "deleteaudio.h"
#include "mainwindow.h"
#include "utils/areyousure/areyousure.h"
#include "ui_mainwindow.h"

#include "../audioplay/aggiornotastiriascolto.h"
void deleteaudio(MainWindow *parent){
    if(!areyousure("", "Are you sure you want to delete the audio?"))
        return;

    parent->m_canvas->data->cleanAudio();

    /*
     * TODO -> remove the file on the zip
    */

    aggiornotestiriascolto(parent);
}

void MainWindow::on_actionDelete_audio_triggered()
{
    deleteaudio(this);
}
