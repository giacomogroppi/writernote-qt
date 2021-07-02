#include "renamefile.h"

#include "../mainwindow.h"
#include "redolist.h"
#include "QInputDialog"
#include "../utils/dialog_critic/dialog_critic.h"
#include "../datawrite/renamefile_f_zip.h"

#include "ui_mainwindow.h"
#include "../dataread/xmlstruct.h"
#include "../datawrite/savefile.h"
#include "../utils/posizione_binario.h"

void renamefile(MainWindow *parent, const QString &namefile){
    const QString &select = parent->ui->listWidgetSX->currentItem()->text();
    const bool checkname = (select == parent->m_currentTitle);
    bool ok;
    QString namecopybook;
    int posizione;
    QStringList __l;

    Document curr;
    savefile __save(parent->m_path, curr);
    xmlstruct xml(&parent->m_path, nullptr, &curr);

    namecopybook = QInputDialog::getText(parent, "Rename",
                                                 (QString)"Rename " + select, QLineEdit::Normal,
                                                 select, &ok);
    if(!ok || namecopybook == "")
        return redolist(parent);

    if(parent->m_indice.titolo.indexOf(namecopybook) != -1)
        return dialog_critic("A file with this name already exists");

    posizione = parent->m_indice.titolo.indexOf(namefile);

    if(xml.loadfile(select+"xml") == ERROR){
        return dialog_critic("We had a problem loading \"" + select +"\"\nPlease retry");
    }

    if(!renamefile_f_zip(parent->m_path, namefile, namecopybook)){
        //dialog_critic("We had a problem changing the name of the file to " + namecopybook);
        return redolist(parent);
    }

    curr.nome_copybook = namecopybook;
    if(curr.m_touch){
        if(!renamefile_f_zip(parent->m_path, POSIZIONEBINARIO(namefile), namecopybook)){
            //dialog_critic("We had a problem changing the name of the file to " + namecopybook);
            return redolist(parent);
        }
    }

    /*TODO -> pdf*/


    parent->m_indice.titolo[posizione] = namecopybook;

    if(__save.savefile_check_indice(&parent->m_indice) != OK){
        renamefile_f_zip(parent->m_path.toUtf8().constData(), namecopybook.toUtf8().constData(), namefile);
        dialog_critic("We had an error saving the index of the file");
        return parent->update_list_copybook();
    }

    if(checkname){
        parent->updateTitle(parent->m_currenttitle);
        parent->m_currentTitle = namecopybook;
    }

    parent->update_list_copybook();
}
