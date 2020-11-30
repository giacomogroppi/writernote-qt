#ifndef CURRENT_TITLE_RENAME_FILE_CPP
#define CURRENT_TITLE_RENAME_FILE_CPP

#include "../mainwindow.h"
#include "redolist.cpp"
#include "QInputDialog"
#include "../dialog_critic.h"
#include "../datawrite/renamefile_f_zip.h"
#include "../update_list_copybook.h"

void renamefile(MainWindow *parent, const char *namefile){
    bool ok;
    QString namecopybook = QInputDialog::getText(parent, "Rename",
                                                 (QString)"Rename " + parent->ui->listWidgetSX->currentItem()->text(), QLineEdit::Normal,
                                                 "", &ok);
    if(!ok || namecopybook == "")
        return redolist(parent);

    if(parent->self->indice.titolo.indexOf(namecopybook.toUtf8().constData()) != -1)
        return dialog_critic("a file with this name already exists");

    int posizione = parent->self->indice.titolo.indexOf(namefile);

    if(!renamefile_f_zip(parent->self->path.c_str(), namefile, namecopybook.toUtf8().constData()))
    {
        dialog_critic(("We had a problem changing the name of the file to " + namecopybook).toUtf8().constData());
        return redolist(parent);
    }

    parent->self->indice.titolo[posizione] = namecopybook;

    return update_list_copybook(parent);
}

#endif //CURRENT_TITLE_RENAME_FILE_CPP
