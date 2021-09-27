#include "mainwindow.h"
#include "utils/dialog_critic/dialog_critic.h"
#include "utils/areyousure/areyousure.h"
#include "dataread/xmlstruct.h"
#include "utils/common_def.h"
#include "ui_mainwindow.h"
#include "filelost.h"
#include "restore_file/get_name_tmp.h"
#include "restore_file/ui/setting_restore_ui.h"
#include "restore_file/restore_file_critic.h"
#include "frompdf/frompdf.h"
#include "last_open/last_open.h"
#include <QFile>

void MainWindow::openFile(const char *pos){
    QString fileName;
    QString path, tmp;
    Document curr;
    xmlstruct xml(&fileName, m_currenttitle);
    n_need_save res_save;

    if(!pos){
        fileName = QFileDialog::getOpenFileName(this, tr("Open File"), "/home/", "Writernote (*." + APP_EXT + ");; Pdf (*.pdf);; All file (* *.*)");
    }else{
        fileName = pos;
    }

    if(fileName == "")
        return;

    QFile file(fileName);
    if (!file.exists())
        return dialog_critic("I can't open the file because it doesn't exist");
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
        return dialog_critic("I can't open this file because of the permission");

    file.close();

#if !(defined(ANDROID_WRITERNOTE) || defined(IOS_WRITERNOTE))
    // check if the file exist
    if(fileName.indexOf(APP_EXT) != -1
            && fileName.indexOf(".pdf") != -1){
        if(!areyousure("Error", "The file does not have the writernote extension, or a pdf extention, do you want to open it anyway?")){
            return;
        }
    }
#endif

    // restore lost file
    if(fileName.indexOf(".pdf") != -1 && fileLost::exe(fileName)){
        if(areyousure("Restore file", "Maybe the last time you opened this file the app closed suddenly, do you want to restore the file?")){

            if(restore_file_critic::restore_file(fileName)){
                user_message("File restore correcty");
            }else{
                return dialog_critic("If not restore correctly");
            }
        }else{
            tmp = get_name_tmp::get(fileName);
            QFile::remove(tmp);
        }
    }


    // check if is pdf or we need to save the current document
    const bool pdf = fileName.indexOf(".pdf") != -1;
    if(pdf){
#ifdef PDFSUPPORT
        m_currenttitle->m_pdf->addPdf(fileName, nullptr, this->m_path, this->m_canvas);
#else
        user_message("Pdf support is not enable in this version");
#endif // PDFSUPPORT
    }
    else {
        res_save = this->needToSave(&xml, &curr);

        /*
         * in case there is already open a file, we need to controll if the
         * current file has beed modified
        */
        if(res_save == n_need_save::need_save){
            if(areyousure("Save need", "Do you want to save " + this->m_path + "?")){
                savefile save(&m_path, m_currenttitle);
                if(save.savefile_check_file() != OK){
                    if(!areyousure("Save fail", "We failed to save the file, should i continue?")){
                        return;
                    }
                }
            }
        }

#if !(defined(ANDROID_WRITERNOTE) || defined(IOS_WRITERNOTE))
        this->m_setting->changeCopybookFile();
#endif

        m_path = fileName;
        const auto res = xml.loadfile(true, true);
        if(res == OK){
            return;
        }else if(res == ERROR_VERSION){
            dialog_critic("You cannot read this file because it was created with a too old version of writernote");
        }else if(res == ERROR_VERSION_NEW){
            dialog_critic("You cannot read this file because it was created with a newer version of writernote");
        }else if(res == ERROR_CONTROLL){
            if(!areyousure("Error opening file", "This file is correct, do you want to open it anyway?")){
                this->m_currenttitle->reset();
                this->m_path = "";
            }
        }else if(res == ERROR_MULTIPLE_COPYBOOK){
            user_message("Writernote has decided to completely change the file format, now it will no longer be possible to have more than one copybook in a file. \nTo use files created with writernote versions lower than or equal to 1.5.2h you must:\nOpen the file with a compressed archive manager, extract all the files that have the same prefix, i.e. the copybook name, and move them into a new writernote file, changing all the names of the same copybook with the prefix data.");
            this->m_path = "";
        }
    }
}

void MainWindow::on_actionOpen_triggered()
{
    openFile(nullptr);
}
