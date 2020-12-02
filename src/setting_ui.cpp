#ifndef SETTING_UI
#define SETTING_UI

#include "setting_ui.h"

#include "ui_mainwindow.h"
#include "mainwindow.h"
#include <QFontComboBox>
#include <QToolBar>
#include <QDebug>

/* il file divide a scaglioni l'attivazione o il disabilitamento dei pulsanti */

void settingstyle(MainWindow *parent, bool check){
    parent->ui->spinBox_fontsize->setEnabled(check);
    parent->ui->fontComboBox_fonttipo->setEnabled(check);
    parent->ui->boldbotton->setEnabled(check);
    parent->ui->insertimagebotton->setEnabled(check);
    parent->ui->listbotton->setEnabled(check);
    parent->ui->inserttablebotton->setEnabled(check);
}

void settingtextedit(MainWindow *parent, bool check){
    parent->ui->textEdit->setEnabled(check);
    parent->ui->actionRedo->setEnabled(check);
    parent->ui->actionCopy->setEnabled(check);
    parent->ui->actionCut->setEnabled(check);
    parent->ui->actionPaste->setEnabled(check);
}

void settingaudio_registrazione(MainWindow *parent, bool check){
    parent->ui->pauserecordingbotton->setEnabled(check);
    parent->ui->startrecording->setEnabled(check);
    parent->ui->stoprecordingbotton->setEnabled(check);
    parent->ui->actionImport_Video->setEnabled(check);

}

void settingaudio_riascolto(MainWindow *parent, bool check){
    parent->ui->actionListen_current_audio->setEnabled(check);
    parent->ui->actionDelete_audio->setEnabled(check);
    parent->ui->actionDelete_copybook->setEnabled(check);
    parent->ui->actionConvert_audio_to_text->setEnabled(check);

    /* pulsanti in basso a sinistra per il riascolto dell'audio */
    parent->ui->startriascoltobotton->setEnabled(check);
    parent->ui->stopriascolto->setEnabled(check);
    parent->ui->audioSlider->setEnabled(check);
    parent->ui->volumeSlider->setEnabled(check);
}

void settingaudio_riascoltoinatto(MainWindow *parent, bool check){
    /* true attivare pulsante di riascolto in basso */
    parent->ui->startriascoltobotton->setEnabled(check);
    parent->ui->stopriascolto->setEnabled(check);
    parent->ui->audioSlider->setEnabled(check);
    parent->ui->volumeSlider->setEnabled(check);


    /* abilitazione o ! per conversione, eliminazione ... */
    parent->ui->actionConvert_audio_to_text->setEnabled(!check);
    parent->ui->actionDelete_audio->setEnabled(!check);
    parent->ui->actionDelete_copybook->setEnabled(!check);
    parent->ui->actionCreate_new_copybook->setEnabled(!check);
    parent->ui->actionListen_current_audio->setEnabled(!check);

    if(!check){
        settingstyle(parent, true);
    }
}

/*Funzione che gestisce l'aggiunta della barra per selezionare il font*/
void setting_ui_start(MainWindow *parent){
    settingaudio_registrazione(parent, false);
    settingstyle(parent, false);
    settingtextedit(parent, false);
    settingaudio_riascolto(parent, false);

    parent->ui->actionPrint->setEnabled(false);
    //parent->ui->actionCreate_new_copybook->setEnabled(false);
    parent->ui->listWidgetSX->setEnabled(false);

}

#endif // SETTING_UI
