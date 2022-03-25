#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QCloseEvent>
#include <QMessageBox>
#include "datawrite/qfilechoose.h"
#include "datawrite/savefile.h"
#include "utils/dialog_critic/dialog_critic.h"
#include "currenttitle/document.h"
#include "dataread/xmlstruct.h"
#include "currenttitle/checksimilecopybook.h"
#include <QSettings>
#include "utils/setting_define.h"
#include "restore_file/ui/setting_restore_ui.h"
#include "last_open/struct_last_file.h"

static void setting_geometry(const QRect &rect)
{
    QSettings setting(ORGANIZATIONAME, APPLICATION_NAME);
    setting.beginGroup(GROUPNAME_GEOMETRY);

    setting.setValue(KEY_GEOMETRY, rect);
    setting.endGroup();
}

static void setting_autosave(bool check)
{
    QSettings setting(ORGANIZATIONAME, APPLICATION_NAME);
    setting.beginGroup(GROUPNAME_REDOUNDO);

    setting.setValue(KEY_REDOUNDO, check);

    setting.endGroup();

}

void MainWindow::closeEvent (QCloseEvent *event)
{
    last_file::save_data(m_path, TYPE_COMPUTER, TYPE_OWNER_YOU, NULL);
    Document tempcopybook;
    xmlstruct temp_lettura(&m_path, &tempcopybook);
    n_need_save __res;
    QString __message;
    QMessageBox::StandardButton resBtn;
    bool check;
    savefile save_(&m_path, _canvas->data);

    __res = needToSave(temp_lettura,
                       tempcopybook);

    if(__res == n_need_save::unable_load){
        QMessageBox msgBox;
        msgBox.setText("Error");
        msgBox.setInformativeText("We had an error opening the current file, do you want to close anyway?");
        msgBox.setStandardButtons(QMessageBox::Ok | QMessageBox::Cancel);
        msgBox.setDefaultButton(QMessageBox::Cancel);

        msgBox.setIcon(QMessageBox::Warning);

        msgBox.setSizeIncrement(300, 10);

        int rec = msgBox.exec();

        if(rec == QMessageBox::Ok){
            goto accept_event;
        }
        return event->ignore();
    }


    if(__res == n_need_save::not_ || __res == n_need_save::only_writernote){
        goto accept_event;
    }


    if(m_path.isEmpty())
        __message = "Do you want to save?";
    else
        __message = "Do you want to save in " + m_path + "?";


    resBtn = QMessageBox::question( this, "Writernote",
                                    __message,
                                    QMessageBox::Cancel | QMessageBox::No | QMessageBox::Yes,
                                    QMessageBox::Yes);

    if (resBtn == QMessageBox::Yes) {

        if(m_path == ""){
            if(!qfilechoose::getFileForSave(m_path, TYPEFILEWRITER))
                return;
        }
        check = save_.savefile_check_file(true) == OK;


        if(check){
            goto accept_event;
        }
        else{
            dialog_critic("We had a problem saving the file, please retry, or change the position of the file");
            return event->ignore();
        }

    }else if (resBtn == QMessageBox::No){
        goto accept_event;
    }

    else
        return event->ignore();

    accept_event:
    setting_geometry(this->geometry());
    setting_autosave((bool) _canvas->_redoundo);


#if !(defined(ANDROID_WRITERNOTE) || defined(IOS_WRITERNOTE))
    m_setting->changeCopybookFile();
#endif

    event->accept();
}
