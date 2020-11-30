#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QMessageBox>

#include "dialog_critic.h"

void dialog_critic(QString stringa){
    QMessageBox msgBox;
    msgBox.setText(stringa);
    msgBox.setIcon(QMessageBox::Critical);
    msgBox.exec();
}

void messaggio_utente(QString stringa){
    QMessageBox msgBox;
    msgBox.setText(stringa);
    msgBox.exec();
}

