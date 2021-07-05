#include "setting_ui.h"

#include "ui_mainwindow.h"
#include "mainwindow.h"
#include <QFontComboBox>
#include <QToolBar>

/*Funzione che gestisce l'aggiunta della barra per selezionare il font*/
void setting_ui_start(MainWindow *parent){
    parent->ui->volumeSlider->setMaximum(100);
    parent->ui->volumeSlider->setMinimum(0);

    parent->setWindowTitle("Writernote");

    parent->contrUi();

    /*settingaudio_registrazione(parent, false);
    settingstyle(parent, false);
    settingtextedit(parent, false);
    settingaudio_riascolto(parent, false);*/

    parent->ui->actionPrint->setEnabled(false);
    parent->ui->stop_rec->setEnabled(false);

}
