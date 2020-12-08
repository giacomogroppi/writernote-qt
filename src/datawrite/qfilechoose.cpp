#ifndef Q_FILE_CHOOSE_CPP
#define Q_FILE_CHOOSE_CPP
#include "qfilechoose.h"
#include <QString>
#include <QDebug>
#include <QFileDialog>
#include "../self_class.h"

#include "../mainwindow.h"
#include "ui_mainwindow.h"

qfilechoose::qfilechoose(MainWindow *ui)
{
    this->ui = ui;
}

bool qfilechoose::filechoose(){
    QString nameFile= QFileDialog::getSaveFileName(this->ui,
        "Save", ".writer", "Image Files (*.writer)");

    if(nameFile == "")
        return false;

    this->ui->self->path = nameFile.toUtf8().constData();
#ifdef STAMPA
    qDebug() << "path: " << this->ui->self->path.c_str();
#endif
    return true;
}

#endif
