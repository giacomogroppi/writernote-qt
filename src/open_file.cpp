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

#include <QFile>

void MainWindow::openFile(const char *pos){
    QString fileName;
    if(!pos){
        fileName = QFileDialog::getOpenFileName(this, tr("Open File"), "/home/", "Writernote (*." + APP_EXT + ");; All file (* *.*)");
    }else{
        fileName = pos;
    }

    if(fileName == "")
        return;

    QFile file(fileName);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
        return dialog_critic("I can't open this file because of the permission");

    file.close();

#ifndef ANDROID
    if(fileName.indexOf(APP_EXT) == -1){
        if(!areyousure(nullptr, "Error", "The file does not have the writernote extension, do you want to open it anyway?")){
            return;
        }
    }
#endif

    if(fileLost::exe(fileName)){
        if(areyousure(nullptr, "Restore file", "Maybe the last time you opened this file the app closed suddenly, do you want to restore the file?")){
            if(restore_file_critic::restore_file(fileName,
                                                 "",
                                                 true)){
                messaggio_utente("File restore correcty");
            }else{
                return dialog_critic("If not restore correctly");
            }
        }else{
            QString tmp = get_name_tmp::get(fileName);
            QFile::remove(tmp);
        }
    }

    xmlstruct filefind(&fileName, &m_indice, m_currenttitle);
    if(!filefind.loadindice())
        return dialog_critic("We had a problem reading the index of the file");

    this->m_setting->changeCopybookFile();

    m_path = fileName;

    if(this->m_indice.titolo.length() > 0)
        this->ui->listWidgetSX->setEnabled(true);
    update_list_copybook();
}

void MainWindow::on_actionOpen_triggered()
{
    openFile(nullptr);
}
