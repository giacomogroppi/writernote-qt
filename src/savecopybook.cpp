#ifndef SAVE_COPY_BOOK_CPP
#define SAVE_COPY_BOOK_CPP

#include <QMessageBox>

#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDebug>
#include "self_class.h"
#include "datawrite/savefile.h"
#include "savecopybook.h"
#include "datawrite/qfilechoose.h"
#include "dialog_critic.h"

/*lib che gestisce il salvaggio di tutti i file*/
#include "datawrite/xmlstruct.h"

savecopybook::savecopybook(MainWindow *ui, QListWidgetItem *position){
    this->ui = ui;
    this->position = position;
}

/* funzione che gestisce il salvataggio del copybook [chiedendo all'utente se vuole salvare il file o no]*/
bool savecopybook::check_permission(){
    QMessageBox msgBox;
    msgBox.setText("The document has been modified.");
    msgBox.setInformativeText("Do you want to save your changes?");
    msgBox.setStandardButtons(QMessageBox::Save | QMessageBox::Discard | QMessageBox::Cancel);
    msgBox.setDefaultButton(QMessageBox::Save);
    int ret = msgBox.exec();

    if(ret == 8388608)
        /*close without saving*/
        return true;
    if(ret == 4194304)
        /*cancel*/
        return false;

    /* else save*/
    if(this->ui->self->path == "")
    {
        qfilechoose pathchoose(this->ui);
        int check = pathchoose.filechoose();

        if(!check) /*vuol dire che l'utente non ha selezionato nessun file o posizione*/
            return false;
    }
    savefile save_class(this->ui, this->position);

    int posizione = this->ui->self->indice.titolo.indexOf(position->text().toUtf8().constData());

    bool check = save_class.savefile_check_indice() && save_class.savefile_check_file(posizione);
    dialog_critic("We had a problem saving the copybook");


    return check;
}

#endif
