#include <QMessageBox>

#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "datawrite/savefile.h"
#include "savecopybook.h"
#include "datawrite/qfilechoose.h"
#include "utils/dialog_critic/dialog_critic.h"

#include "utils/areyousure/areyousure.h"

#include "dataread/xmlstruct.h"

savecopybook::savecopybook(MainWindow *parent_U, QString *namecopybook_U){
    this->parent = parent_U;
    this->namecopybook = namecopybook_U;
}

/* funzione che gestisce il salvataggio del copybook [chiedendo all'utente se vuole salvare il file o no]*/
bool savecopybook::check_permission(){
    QMessageBox msgBox;
    msgBox.setText("The document has been modified.");
    msgBox.setInformativeText("Do you want to save your changes?");
    msgBox.setStandardButtons(QMessageBox::Save | QMessageBox::Discard | QMessageBox::Cancel);
    msgBox.setDefaultButton(QMessageBox::Save);
    int ret = msgBox.exec();

    if(ret == QMessageBox::Discard)
        /*close without saving*/
        return true;
    if(ret == QMessageBox::Cancel)
        /*cancel*/
        return false;

    /* else save*/
    if(parent->m_path == "")
    {
        qfilechoose pathchoose(this->parent);
        int check = pathchoose.filechoose(&parent->m_path);

        if(!check) /*vuol dire che l'utente non ha selezionato nessun file o posizione*/
            return false;
    }
    savefile save_class(&this->parent->m_path, this->parent->m_currenttitle);

    bool check = save_class.savefile_check_indice(&parent->m_indice)==OK && save_class.savefile_check_file()==OK;
    if(!check)
        dialog_critic("We had a problem saving the copybook");


    return check;
}

