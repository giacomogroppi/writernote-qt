#include "../mainwindow.h"
#include "../utils/get_file_dir/get_file_dir.h"
#include "../utils/path/get_path.h"
#include "../restore_file/ui/setting_restore_ui.h"
#include "../utils/common_def.h"
#include "../utils/dialog_critic/dialog_critic.h"
#include "restore_file_critic.h"
#include "../datawrite/qfilechoose.h"
#include "../dataread/xmlstruct.h"
#include "../utils/areyousure/areyousure.h"

#include <QMessageBox>

void MainWindow::resFileTmpInTmpFolder(){
    QStringList __l;
    QString __pos;
    QMessageBox::StandardButton resBtn;
    const int pos_ext = QString("." + APP_EXT).length();
    QString __path;
    restore_file_critic::n_err __res;
    uint i, len, count;

    __pos = get_path(path::tmp_file_not_save);
    __l = get_file_dir::get(__pos);

    len = __l.length();
    if(!len){
        return;
    }

    QString tmp = __l.at(0).mid(__l.at(0).length() - pos_ext);

    for(i=0, count=0; i<len; ++i){
        if(__l.at(i).length() > pos_ext
                && __l.at(i).mid(__l.at(i).length() - pos_ext) == "."+APP_EXT){
            count++;
        }else{
            __l.removeAt(i);
            len--;
            i--;
        }
    }

    /*
     * the only file in the folder is the current
    */
    if(count == 1 && __l.first() == m_setting->get_curr_name_tmp()){
        return;
    }

    for(i=0; i<len; ++i){

        const QString &res = __l.at(i);

        resBtn = QMessageBox::question(nullptr, "File lost",
                                        "Do you want to restore this file" + res + "?", QMessageBox::Ignore | QMessageBox::Yes | QMessageBox::Help,
                                        QMessageBox::Yes);

        if(resBtn == QMessageBox::Help){
            messaggio_utente("Last time you used writernote, it probably quit unexpectedly, if you want to recover the file, click YES, otherwise Ignore to delete the temporary file.");
            --i;
            continue;
        }

        if(resBtn == QMessageBox::Yes){
            if(!qfilechoose::filechoose(__path, TYPEFILEWRITER)){
                --i;
            }

            __res = restore_file_critic::restore_file_direct(res, __path, NAME_UNSAVE);

            if(__res == restore_file_critic::error_load_file){
                if(areyousure("Error", "Error loading the file " + res + "\nDelete the file?")){
                    QFile::remove(res);
                }
            }

            if(__res == restore_file_critic::error_save_file){
                if(!areyousure("Error", "Error saving the file.")){
                    i--;
                }
            }

            if(__res == restore_file_critic::restore_ok){
                messaggio_utente("Saved in " + res);
            }
        }

    }

    for(i=0; i<len; ++i){
        QFile::remove(__l.at(i));
    }

}
