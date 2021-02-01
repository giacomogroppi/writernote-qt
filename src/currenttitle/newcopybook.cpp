#include "../mainwindow.h"
#include "ui_mainwindow.h"

#include <QInputDialog>
#include "../datawrite/qfilechoose.h"
#include "../dialog_critic.h"
#include "../newcopybook_.h"
#include "../update_list_copybook.h"

#include "../touch/datastruct/datastruct.h"

void MainWindow::methodpen(){
    this->typetemp = true;
}

void MainWindow::methodwrite(){
    /* per ora niente */
    this->typetemp = false;
}

/* funzione che gestisce la creazione di un nuovo copybook */
void MainWindow::on_actionCreate_new_copybook_triggered()
{
    /* chiede all'utente se vuole creare un nuovo fai per scrivere a tastiera o con la penna */
    QMenu *menu = new QMenu(this);
    menu->setTitle("Choose input method");

    QAction *draw = new QAction; // Assumes actions is not empty
    draw->setStatusTip(tr("Draw with your pen [Alpha]"));
    draw->setText("Draw area [beta]");
    menu->addAction(draw);

    QAction *keyboard = new QAction;
    keyboard->setStatusTip("Write with your keyboard");
    keyboard->setText("Plain text");
    menu->addAction(keyboard);

    connect(draw, &QAction::triggered, this, &MainWindow::methodpen);
    connect(keyboard, &QAction::triggered, this, &MainWindow::methodwrite);


    auto hostRect = this->cursor().pos();
    menu->move(hostRect.x(), hostRect.y());

    /* if the user didn't click on the menu */
    if(!menu->exec())
        return;

    bool ok;
    if(this->self->path == "")
    {
        /* deve salvare prima di continuare */
        qfilechoose filec(this);
        ok = filec.filechoose(&self->path);
        if (!ok){
            //goto stop;
            return;
        }
    }

    /* richiede all'utente se vuole salvare il file */
    QString namecopybook = QInputDialog::getText(this, tr("Get text"),
                                                 tr("Title: "), QLineEdit::Normal,
                                                 "", &ok);

    if(!ok || namecopybook == "")
        return;

    if(this->self->indice.titolo.indexOf(namecopybook) != -1)
        return dialog_critic("There is a copybook that already has this title");


    if(!newcopybook_(this, namecopybook))
        return dialog_critic("We had a problem saving the copybook");

    this->ui->listWidgetSX->setEnabled(true);
    update_list_copybook(this);

/*stop:{
    delete menu;
    delete keyboard;
    delete draw;
    }*/
}
