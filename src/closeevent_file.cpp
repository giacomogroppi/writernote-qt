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

#include "last_open/struct_last_file.h"

static void setting_hide_list(bool check);
static void setting_geometry(QRect );

static void setting_autosave(bool check){
    QSettings setting(ORGANIZATIONAME, APPLICATION_NAME);
    setting.beginGroup(GROUPNAME_REDOUNDO);

    setting.setValue(KEY_REDOUNDO, check);

    setting.endGroup();

}

#define accept_event(x) setting_geometry(this->geometry()); \
    setting_hide_list(ui->listWidgetSX->isHidden()); \
    setting_autosave(enableredoundo); \
    return x->accept();

void MainWindow::closeEvent (QCloseEvent *event)
{
    if(!m_indice.titolo.length() || m_currentTitle == ""){
        accept_event(event);
    }

    /*
     * TODO: after enable cloud support,
     * pass the right value
    */
    save_data(m_path, TYPE_COMPUTER, TYPE_OWNER_YOU, NULL);

    /* apre il file in file e lo carica nell'oggetto, e lo confronta */
    currenttitle_class tempcopybook;
    indice_class tempindice;

    xmlstruct temp_lettura(&m_path, &tempindice, &tempcopybook);

    if(temp_lettura.loadfile((m_currentTitle + ".xml").toUtf8().constData()) != OK
            || !temp_lettura.loadindice()){

        QMessageBox msgBox;
        msgBox.setText("Error:");
        msgBox.setInformativeText("We had an error opening the current file");
        msgBox.setStandardButtons(QMessageBox::Ok | QMessageBox::Cancel);
        msgBox.setDefaultButton(QMessageBox::Cancel);

        msgBox.setIcon(QMessageBox::Warning);

        msgBox.setSizeIncrement(300, 10);

        int rec = msgBox.exec();

        if(rec == QMessageBox::Ok){
            accept_event(event);
        }
        return event->ignore();
    }

    QString filep, __message ;

    /*
     * if we are in keyboard mode
    */
    if(!m_currenttitle->m_touch){
        filep = m_currenttitle->testi;
        m_currenttitle->testi = this->ui->textEdit->toHtml();
    }

    bool check1 = checksimilecopybook(&tempcopybook, m_currenttitle, false) == OK_CHECK;

    check1 = check1 && (checksimileindice(&m_indice, &tempindice) == OK_CHECK);

    /* if all is equal close app */
    if(check1){
        accept_event(event);
    }


    if(m_currenttitle->m_touch){
        if(!m_currenttitle->datatouch->userWrittenSomething()){
            accept_event(event);
        }
    }
    else{
        m_currenttitle->testi = filep;
    }


    if(m_path.isEmpty())
        __message = "Do you want to save?";
    else
        __message = "Do you wanto to save in " + m_path + "?";


    QMessageBox::StandardButton resBtn = QMessageBox::question( this, "writernote",
                                                                __message,
                                                                QMessageBox::Cancel | QMessageBox::No | QMessageBox::Yes,
                                                                QMessageBox::Yes);

    if (resBtn == QMessageBox::Yes) {

        if(m_path == ""){
            qfilechoose file(this);
            if(!file.filechoose(&m_path))
                return;
        }

        savefile save_(&m_path, m_currenttitle);

        bool check = save_.savefile_check_indice(&m_indice)==OK;

        if(m_currentTitle != "")
            check = check && (save_.savefile_check_file()==OK);


        if(check){
            accept_event(event);
        }
        else{
            dialog_critic((QString)"We had a problem saving the file, please retry");
            return event->ignore();
        }

    } else if (resBtn == QMessageBox::No)
    {
        return accept_event(event);
    }

    else
        return event->ignore();

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
