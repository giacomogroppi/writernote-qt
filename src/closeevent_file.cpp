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

static void setting_autosave(bool check){
    QSettings setting(ORGANIZATIONAME, APPLICATION_NAME);
    setting.beginGroup(GROUPNAME_REDOUNDO);

    setting.setValue(KEY_REDOUNDO, check);

    setting.endGroup();

}

void MainWindow::closeEvent (QCloseEvent *event)
{
    QSettings setting(ORGANIZATIONAME, APPLICATION_NAME);
    setting.beginGroup(GROUPNAME_GEOMETRY);

    setting.setValue(KEY_GEOMETRY, this->geometry());
    setting.endGroup();

    setting_hide_list(ui->listWidgetSX->isHidden());
    setting_autosave(enableredoundo);

    if(!m_indice.titolo.length() || m_currentTitle == "")
        return event->accept();

    /*
     * TODO: after create cloud instance, pass the right
     * value
    */
    save_data(m_path, TYPE_COMPUTER, TYPE_OWNER_YOU, NULL);

    /* apre il file in file e lo carica nell'oggetto, e lo confronta */
    currenttitle_class *tempcopybook = new currenttitle_class;
    indice_class *tempindice = new indice_class;

    xmlstruct temp_lettura(&m_path, tempindice, tempcopybook);

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

        delete tempcopybook;
        delete tempindice;

        if(rec == QMessageBox::Ok){
            return event->accept();
        }
        return event->ignore();
    }

    QString filep;

    if(m_currenttitle.m_touch){
        filep = m_currenttitle.testi;
        m_currenttitle.testi = this->ui->textEdit->toHtml();
    }

    bool check1 = checksimilecopybook(tempcopybook, &m_currenttitle) == OK;

    check1 = check1 && checksimileindice(&m_indice, tempindice);

    /* se è uguale sia il copybook che l'indice accetta */
    if(check1){
        delete tempcopybook;
        delete temp_lettura;
        delete tempindice;
        return event->accept();
    }


    if(m_currenttitle.m_touch)
        m_currenttitle.testi = filep;


    QMessageBox::StandardButton resBtn = QMessageBox::question( this, "writernote",
                                                                tr("Do you want to save\n"),
                                                                QMessageBox::Cancel | QMessageBox::No | QMessageBox::Yes,
                                                                QMessageBox::Yes);

    if (resBtn == QMessageBox::Yes) {

        if(m_path == ""){
            qfilechoose file(this);
            if(!file.filechoose(&m_path))
                return;
        }

        savefile save_(&m_path, &m_currenttitle);

        bool check = save_.savefile_check_indice(&m_indice)==OK;

        if(m_currentTitle != "")
            check = check && (save_.savefile_check_file()==OK);


        if(check)
            return event->accept();

        else{
            dialog_critic((QString)"We had a problem saving the file, please retry");
            return event->ignore();
        }

    } else if (resBtn == QMessageBox::No)
        return event->accept();

    else
        return event->ignore();

    delete tempcopybook;
    delete tempindice;
    delete temp_lettura;
}

static void setting_hide_list(bool check){
    QSettings setting(ORGANIZATIONAME, APPLICATION_NAME);
    setting.beginGroup(GROUPNAME_LIST_HIDDEN);

    setting.setValue(KEY_LIST_HIDDEN, check);

    setting.endGroup();
}
