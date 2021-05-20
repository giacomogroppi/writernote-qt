#include "renamefile.h"

#include "../mainwindow.h"
#include "redolist.h"
#include "QInputDialog"
#include "../utils/dialog_critic/dialog_critic.h"
#include "../datawrite/renamefile_f_zip.h"

#include "ui_mainwindow.h"

#include "../datawrite/savefile.h"

void renamefile(MainWindow *parent, const char *namefile){
    bool checkname = (parent->ui->listWidgetSX->currentItem()->text() == parent->m_currentTitle);

    bool ok;
    QString namecopybook = QInputDialog::getText(parent, "Rename",
                                                 (QString)"Rename " + parent->ui->listWidgetSX->currentItem()->text(), QLineEdit::Normal,
                                                 parent->ui->listWidgetSX->currentItem()->text(), &ok);
    if(!ok || namecopybook == "")
        return redolist(parent);

    if(parent->m_indice.titolo.indexOf(namecopybook.toUtf8().constData()) != -1)
        return dialog_critic("a file with this name already exists");

    int posizione = parent->m_indice.titolo.indexOf(namefile);

    if(!renamefile_f_zip(parent->m_path.toUtf8().constData(), namefile, namecopybook.toUtf8().constData())){
        dialog_critic(("We had a problem changing the name of the file to " + namecopybook).toUtf8().constData());
        return redolist(parent);
    }

    parent->m_indice.titolo[posizione] = namecopybook;

    savefile file_(&parent->m_path, NULL);

    if(file_.savefile_check_indice(&parent->m_indice) != OK){
        renamefile_f_zip(parent->m_path.toUtf8().constData(), namecopybook.toUtf8().constData(), namefile);
        dialog_critic("We had an error saving the index of the file");
        return parent->update_list_copybook();
    }

    parent->setWindowTitle("Writernote - " + namecopybook);
    if(checkname)
        parent->m_currentTitle = namecopybook;

    return parent->update_list_copybook();
}
