#ifndef SETTING_UI_H
#define SETTING_UI_H

#include "mainwindow.h"

void settingstyle(MainWindow *parent, bool check);

void settingtextedit(MainWindow *parent, bool check);

void settingaudio_registrazione(MainWindow *parent, bool check);

/*Funzione che gestisce l'aggiunta della barra per selezionare il font*/
void setting_ui_start(MainWindow *parent);
void settingaudio_riascoltoinatto(MainWindow *, bool);
#endif // SETTING_UI_H
