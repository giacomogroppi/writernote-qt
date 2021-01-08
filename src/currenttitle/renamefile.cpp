#include "renamefile.h"

#include "../mainwindow.h"
#include "redolist.h"
#include "QInputDialog"
#include "../dialog_critic.h"
#include "../datawrite/renamefile_f_zip.h"
#include "../update_list_copybook.h"

#include "ui_mainwindow.h"

#include "../datawrite/savefile.h"

void renamefile(MainWindow *parent, const char *namefile){
    bool checkname = (parent->ui->listWidgetSX->currentItem()->text() == parent->self->currentTitle);

    bool ok;
    QString namecopybook = QInputDialog::getText(parent, "Rename",
                                                 (QString)"Rename " + parent->ui->listWidgetSX->currentItem()->text(), QLineEdit::Normal,
                                                 parent->ui->listWidgetSX->currentItem()->text(), &ok);
    if(!ok || namecopybook == "")
        return redolist(parent);

    if(parent->self->indice.titolo.indexOf(namecopybook.toUtf8().constData()) != -1)
        return dialog_critic("a file with this name already exists");

    int posizione = parent->self->indice.titolo.indexOf(namefile);

    if(!renamefile_f_zip(parent->self->path.toUtf8().constData(), namefile, namecopybook.toUtf8().constData())){
        dialog_critic(("We had a problem changing the name of the file to " + namecopybook).toUtf8().constData());
        return redolist(parent);
    }

    parent->self->indice.titolo[posizione] = namecopybook;

    /* non c'è bisogno di passargli l'oggetto della classe in quanto salva solamente l'indice */
    savefile file_(parent, nullptr, &parent->self->currentTitle);

    if(!file_.savefile_check_indice()){
        renamefile_f_zip(parent->self->path.toUtf8().constData(), namecopybook.toUtf8().constData(), namefile);
        dialog_critic("We had an error saving the index of the file");
        return update_list_copybook(parent);
    }

    parent->setWindowTitle("Writernote - " + namecopybook);
    if(checkname)
        parent->self->currentTitle = namecopybook;

    return update_list_copybook(parent);
}
