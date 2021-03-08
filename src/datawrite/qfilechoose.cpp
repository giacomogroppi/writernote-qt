#include "qfilechoose.h"
#include <QString>
#include <QFileDialog>

#include "../mainwindow.h"
#include "ui_mainwindow.h"

/*
 * this function is use for
 * save [not for loading]
*/

qfilechoose::qfilechoose(MainWindow *ui)
{
    this->ui = ui;
}

bool qfilechoose::filechoose(QString *nome, short int type_){
    QString extention;
    QString type;

    if(type_ == TYPEFILEWRITER){
        type = "File Writer (*.writer)";
        extention = ".writer";
    }
    else if(type_ == TYPEFILEPDF){
        type = "PDF (*.pdf)";
        extention = ".pdf";
    }
    else if(type_ == TYPEAUDIO){
        type = "Audio (*.wav)";
        extention = ".wav";
    }

    QString nameFile= QFileDialog::getSaveFileName(this->ui,
        "Save", extention, type);

    if(nameFile == "")
        return false;

    *nome = nameFile;

    return true;
}
