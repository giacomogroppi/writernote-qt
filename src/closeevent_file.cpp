#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QCloseEvent>
#include <QMessageBox>

#include "datawrite/qfilechoose.h"
#include "datawrite/savefile.h"
#include "utils/dialog_critic/dialog_critic.h"

#include "currenttitle/currenttitle_class.h"

#include "dataread/xmlstruct.h"
#include "currenttitle/checksimilecopybook.h"

#include <QSettings>
#include "utils/setting_define.h"
#include "restore_file/ui/setting_restore_ui.h"
#include "last_open/struct_last_file.h"

static void setting_hide_list(bool check);
static void setting_geometry(QRect );

static void setting_autosave(bool check){
    QSettings setting(ORGANIZATIONAME, APPLICATION_NAME);
    setting.beginGroup(GROUPNAME_REDOUNDO);

    setting.setValue(KEY_REDOUNDO, check);

    setting.endGroup();

}

void MainWindow::closeEvent (QCloseEvent *event)
{
    /*
     * TODO: after enable cloud support,
     * pass the right value
    */
    save_data(m_path, TYPE_COMPUTER, TYPE_OWNER_YOU, NULL);

    /* apre il file in file e lo carica nell'oggetto, e lo confronta */
    currenttitle_class tempcopybook;
    indice_class tempindice;

    xmlstruct temp_lettura(&m_path, &tempindice, &tempcopybook);

    n_need_save __res;

    QString filep, __message;

    QMessageBox::StandardButton resBtn;
    bool check;
    savefile save_(&m_path, m_currenttitle);
    qfilechoose file(nullptr);

    if(!m_indice.titolo.length() || m_currentTitle == ""){
        goto accept_event;
    }

    __res = needToSave(&temp_lettura,
                       &tempcopybook,
                       &tempindice);

    if(__res == n_need_save::unable_load){
        QMessageBox msgBox;
        msgBox.setText("Error:");
        msgBox.setInformativeText("We had an error opening the current file");
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


    if(!m_currenttitle->m_touch){
        filep = m_currenttitle->testi;
        m_currenttitle->testi = this->ui->textEdit->toHtml();
    }

    if(__res == n_need_save::not_
            || __res == n_need_save::only_writernote){
        goto accept_event;
    }


    if(m_path.isEmpty())
        __message = "Do you want to save?";
    else
        __message = "Do you wanto to save in " + m_path + "?";


    resBtn = QMessageBox::question( this, "writernote",
                                    __message,
                                    QMessageBox::Cancel | QMessageBox::No | QMessageBox::Yes,
                                    QMessageBox::Yes);

    if (resBtn == QMessageBox::Yes) {

        if(m_path == ""){
            qfilechoose file(nullptr);
            if(!file.filechoose(&m_path))
                return;
        }

        check = save_.savefile_check_indice(&m_indice)==OK;

        if(m_currentTitle != "")
            check = check && (save_.savefile_check_file()==OK);


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
    setting_hide_list(ui->listWidgetSX->isHidden());
    setting_autosave(enableredoundo);

    m_setting->changeCopybookFile();

    event->accept();
}

static void setting_hide_list(bool check){
    QSettings setting(ORGANIZATIONAME, APPLICATION_NAME);
    setting.beginGroup(GROUPNAME_LIST_HIDDEN);

    setting.setValue(KEY_LIST_HIDDEN, check);

    setting.endGroup();
}

static void setting_geometry(QRect rect){
    QSettings setting(ORGANIZATIONAME, APPLICATION_NAME);
    setting.beginGroup(GROUPNAME_GEOMETRY);

    setting.setValue(KEY_GEOMETRY, rect);
    setting.endGroup();
}
