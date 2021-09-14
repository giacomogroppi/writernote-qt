#include "qfilechoose.h"
#include <QString>
#include <QFileDialog>

#include "../mainwindow.h"
#include "ui_mainwindow.h"
#include "../utils/common_def.h"
#include "../utils/get_path_application.h"
#include "../utils/get_file_dir/get_file_dir.h"
#include "../utils/common_def.h"

/*
 * this function is use for
 * save [not for loading]
*/

static void removeNotWriternote(QStringList &ref, const QString &ext);

bool qfilechoose::filechoose(QString &nome, short int type_){
    QString extention;
    QString type;
    QString dir, nameFile;
#ifdef ANDROID
    QStringList list;
#endif // ANDROID

    if(type_ == TYPEFILEWRITER){
        type = "File Writer (*." + APP_EXT + ")";
        extention = ".writer";
    }
    else if(type_ == TYPEFILEPDF){
        type = "PDF (*.pdf)";
        extention = ".pdf";
    }
    else if(type_ == TYPEAUDIO){
        type = "Audio (*.wav)";
        extention = ".wav";
    } else if(type_ == TYPELOG){
#ifndef ANDROID
        dir = QFileDialog::getExistingDirectory(nullptr, "Open Directory",
                                                        "",
                                                        QFileDialog::ShowDirsOnly
                                                        | QFileDialog::DontResolveSymlinks);
#else

#endif
        if(dir == "")
            return false;

        nome = dir;
        return true;
    }
#ifndef ANDROID
    nameFile= QFileDialog::getSaveFileName(nullptr,
        "Save", extention, type);
#else
    dir = get_path_application::exe();
    list = get_file_dir::get(dir);

    removeNotWriternote(list, extention);
#endif

    if(nameFile == "")
        return false;

    nome = nameFile;

    return true;
}

static void removeNotWriternote(QStringList &ref, const QString &ext){
    int i;

    for(i=0; i<ref.length(); ++i){
        if(ref.at(i).indexOf(ext) != -1){
            ref.removeAt(i);
        }
    }
}
