#include "../mainwindow.h"
#include "../utils/get_file_dir/get_file_dir.h"
#include "../utils/path/get_path.h"
#include "../restore_file/ui/setting_restore_ui.h"
#include "../utils/common_def.h"

#include <QMessageBox>

void MainWindow::resFileTmpInTmpFolder(){
    QStringList __l;
    QString __pos;
    QMessageBox::StandardButton resBtn;

    uint i, len, count;

    __pos = get_path(path::tmp_file_not_save);
    __l = get_file_dir::get(__pos);

    len = __l.length();
    if(!len){
        return;
    }

    for(i=0, count=0; i<len; ++i){
        if(__l.at(i).indexOf("."+APP_EXT) != -1){
            count++;
        }else{
            __l.removeAt(i);
            --len;
            --i;
        }
    }

    /*
     * the only file in the folder is the current
    */
    if(count == 1 && __l.first() == m_setting->get_curr_name_tmp()){
        return;
    }

    for(i=0; i<len; ++i){
        resBtn = QMessageBox::question(nullptr, "File lost",
                                        "Do you want to restore this file" + __l.at(i) + "?", QMessageBox::Discard | QMessageBox::Yes,
                                        QMessageBox::Yes);




    }
}
