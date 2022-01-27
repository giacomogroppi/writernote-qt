#include <QMessageBox>

#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "datawrite/savefile.h"
#include "savecopybook.h"
#include "datawrite/qfilechoose.h"
#include "utils/dialog_critic/dialog_critic.h"

#include "utils/areyousure/areyousure.h"

#include "dataread/xmlstruct.h"

savecopybook::savecopybook(MainWindow *parent_U){
    this->parent = parent_U;
}

/* funzione che gestisce il salvataggio del copybook [chiedendo all'utente se vuole salvare il file o no]*/
bool savecopybook::check_permission()
{
    QMessageBox msgBox;
    int ret;
    savefile save_class(&this->parent->m_path, this->parent->m_canvas->data);
    QString SaveMessage;

    msgBox.setText("The document has been modified.");

    if(parent->m_path.isEmpty()){
        SaveMessage = "Do you want to save your changes?";
    }else{
        SaveMessage = QString("Do you want to save your changes in ?").arg(parent->m_path);
    }

    msgBox.setInformativeText(SaveMessage);

    msgBox.setStandardButtons(QMessageBox::Save | QMessageBox::Discard | QMessageBox::Cancel);
    msgBox.setDefaultButton(QMessageBox::Save);
    ret = msgBox.exec();

    if(ret == QMessageBox::Discard){
        /*close without saving*/
        return true;
    }
    if(ret == QMessageBox::Cancel){
        /*cancel*/
        return false;
    }

    /* else save*/
    if(parent->m_path.isEmpty()){
        if(!qfilechoose::getFileForSave(parent->m_path, TYPEFILEWRITER))
            return false;
    }

    ret = save_class.savefile_check_file() == OK;
    if(!ret)
        dialog_critic("We had a problem saving the copybook");

    return ret;
}

