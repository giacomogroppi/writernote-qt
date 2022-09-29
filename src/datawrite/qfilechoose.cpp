#include "qfilechoose.h"
#include <QString>
#include <QFileDialog>

#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "utils/common_def.h"
#include "utils/get_path_application.h"
#include "utils/get_file_dir/get_file_dir.h"
#include "utils/common_def.h"
#include "utils/removenotwriternote.h"
#include "utils/common_def.h"

/*
 * this function is use for
 * save [not for loading]
*/

bool qfilechoose::getFileForSave(QByteArray &nome, uint16_t type_)
{
    QString extention;
    QString type;
    QString dir, nameFile;
#if defined(ANDROID_WRITERNOTE) || defined(IOS_WRITERNOTE)
    QStringList list;
#endif // ANDROID IOS

    if(type_ == TYPEFILEWRITER){
        type = qstr("File Writer (*.%1)").arg(APP_EXT);
        extention = qstr("." APP_EXT);
    }
    else if(type_ == TYPEFILEPDF){
        type = "PDF (*.pdf)";
        extention = ".pdf";
    }
    else if(type_ == TYPEAUDIO){
        type = "Audio (*.wav)";
        extention = ".wav";
    } else if(type_ == TYPELOG){
#if !(defined(ANDROID_WRITERNOTE) || defined(IOS_WRITERNOTE))
        dir = QFileDialog::getExistingDirectory(nullptr, "Open Directory",
                                                        "",
                                                        QFileDialog::ShowDirsOnly
                                                        | QFileDialog::DontResolveSymlinks);
#else

#endif
        if(dir == "")
            return false;

        nome = dir.toUtf8();
        return true;
    }
#if !(defined(ANDROID_WRITERNOTE) || defined(IOS_WRITERNOTE))
    nameFile = QFileDialog::getSaveFileName(nullptr,
        "Save", extention, type);
#else
    dir = get_path_application::exe();
    list = get_file_dir::get(dir);

    removeNotWriternote::exe(list);
#endif

    if(nameFile == "")
        return false;

    nome = nameFile.toUtf8();

    return true;
}

bool qfilechoose::getFileForLoad(QByteArray &nome, uint16_t type_)
{
    QString extention;
    QString type = "";
    QString dir, nameFile;
#if defined(ANDROID_WRITERNOTE) || defined(IOS_WRITERNOTE)
    QStringList list;
#endif // ANDROID IOS

    if(type_ & TYPEFILEWRITER){
        type += QString("File Writer (*.%1);;").arg(APP_EXT);
        extention = qstr("." APP_EXT);
    }
    if(type_ & TYPEFILEPDF){
        type += QString("PDF (*.pdf);;");
        //type += "PDF (*.pdf)";
        extention += ".pdf";
    }
    if(type_ & TYPEAUDIO){
        type += "Audio (*.wav);;";
        extention = ".wav";
    }
    if(type_ & TYPELOG){
#if !(defined(ANDROID_WRITERNOTE) || defined(IOS_WRITERNOTE))
        dir = QFileDialog::getExistingDirectory(nullptr, "Open Directory",
                                                        "",
                                                        QFileDialog::ShowDirsOnly
                                                        | QFileDialog::DontResolveSymlinks);
#else
        std::abort();
#endif
        if(dir == "")
            return false;

        nome = dir.toUtf8();
        return true;
    }

    if(type_ & TYPEALL){
        type += "All file (* *.*)";
        extention += "";
    }

#if !(defined(ANDROID_WRITERNOTE) || defined(IOS_WRITERNOTE))
    nameFile = QFileDialog::getOpenFileName(nullptr,
        "Open", extention, type);
#else
    dir = get_path_application::exe();
    list = get_file_dir::get(dir);

    removeNotWriternote::exe(list);
#endif

    if(nameFile == "")
        return false;

    nome = nameFile.toUtf8();

    return true;
}
