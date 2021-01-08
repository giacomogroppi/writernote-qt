#include "../mainwindow.h"
#include "qfilechoose.h"

#include "savefile.h"
#include "../dialog_critic.h"

static inline const char *stringa(QString *nomevecchio, QString *nomenuovo){
    return ("cp " + *nomevecchio + " " + *nomenuovo).toUtf8().constData();
}

static bool c = true;

/* funzione che viene richiamata quando viene salvato */
void MainWindow::on_actionSave_File_triggered()
{


    if(self->currenttitle.posizione_binario == "")
        this->self->currenttitle.testi = this->ui->textEdit->toHtml();

    savefile savefile_i(this, &this->self->currenttitle, &self->currentTitle);

    bool check = savefile_i.savefile_check_indice();

    if(this->self->currentTitle != "")
        check = check && savefile_i.savefile_check_file(this->self->indice.titolo.indexOf(this->self->currentTitle));

    if(!check && c)
        return dialog_critic((QString) "We had a problem while saving the file");

    c = true;
}

void MainWindow::on_actionSave_As_triggered()
{
    QString posizione = this->self->path;

    qfilechoose file(this);
    if(!file.filechoose(&self->path))
        /* se l'utente non ha selezionato nessun file */
       return;

    if(posizione != ""){
        int command;
#if defined(unix)
        command = system(stringa(&posizione, &this->self->path));
#endif
        if(command){
            dialog_critic("We had a problem saving a copy of " + posizione + " to " + self->path);
            self->path = posizione;
            return;
        }

        c = false;

        /*save the file in the new path -> keeping the current file the same*/
        this->on_actionSave_File_triggered();
        if(c){
            dialog_critic("We had a problem saving the copybook in " + self->path);
        }
        self->path = posizione;
    }
    else
        return this->on_actionSave_File_triggered();

}
