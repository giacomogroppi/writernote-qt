#include "../mainwindow.h"
#include "qfilechoose.h"

#include "savefile.h"
#include "../utils/dialog_critic/dialog_critic.h"

static inline const char *stringa(QString *nomevecchio, QString *nomenuovo){
    return ("cp " + *nomevecchio + " " + *nomenuovo).toUtf8().constData();
}

static bool c = true;

/* funzione che viene richiamata quando viene salvato */
void MainWindow::on_actionSave_File_triggered()
{
    if(m_currenttitle.m_touch)
        this->m_currenttitle.testi = this->ui->textEdit->toHtml();

    savefile savefile_i(&m_path, &m_currenttitle);

    bool check = savefile_i.savefile_check_indice(&m_indice) == OK;

    if(this->m_currentTitle != "")
        check = check && (savefile_i.savefile_check_file()==OK);

    if(!check && c)
        return dialog_critic((QString) "We had a problem while saving the file");

    c = true;
}

void MainWindow::on_actionSave_As_triggered()
{
    QString posizione = this->m_path;

    qfilechoose file(this);
    if(!file.filechoose(&m_path))
        /* se l'utente non ha selezionato nessun file */
       return;

    if(posizione != ""){
        int command;
#if defined(unix)
        command = system(stringa(&posizione, &m_path));
#endif
        if(command){
            dialog_critic("We had a problem saving a copy of " + posizione + " to " + m_path);
            m_path = posizione;
            return;
        }

        c = false;

        /*save the file in the new path -> keeping the current file the same*/
        this->on_actionSave_File_triggered();
        if(c){
            dialog_critic("We had a problem saving the copybook in " + m_path);
        }
        m_path = posizione;
    }
    else
        return this->on_actionSave_File_triggered();

}
