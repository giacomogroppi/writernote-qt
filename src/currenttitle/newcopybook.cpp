#include "../mainwindow.h"
#include "ui_mainwindow.h"

#include <QInputDialog>
#include "../datawrite/qfilechoose.h"
#include "../utils/dialog_critic/dialog_critic.h"
#include "../newcopybook_.h"
#include "../update_list_copybook.h"

#include "../touch/datastruct/datastruct.h"
#include "default_type/default_type.h"

void MainWindow::methodpen(){
    this->typetemp = true;
}

void MainWindow::methodwrite(){
    /* per ora niente */
    this->typetemp = false;
}

static QMenu *menu = nullptr;

/*
 * manage creation new copybook
 */
void MainWindow::on_actionCreate_new_copybook_triggered()
{
    auto __res = default_type::load_preference_copybook();
    if(__res == default_type::not_set){
        if(!menu){
            menu = new QMenu(this);
            menu->setTitle("Choose input method");

            QAction *draw = new QAction(menu); // Assumes actions is not empty
            draw->setStatusTip(tr("Draw with your pen [Alpha]"));
            draw->setText("Draw area [beta]");
            menu->addAction(draw);

            QAction *keyboard = new QAction(menu);
            keyboard->setStatusTip("Write with your keyboard");
            keyboard->setText("Plain text");
            menu->addAction(keyboard);

            connect(draw, &QAction::triggered, this, &MainWindow::methodpen);
            connect(keyboard, &QAction::triggered, this, &MainWindow::methodwrite);
        }

        QPoint hostRect = this->cursor().pos();
        menu->move(hostRect);

        /* if the user didn't click on the menu */
        if(!menu->exec())
            return;
    }
    else if(__res == default_type::pen){
        this->methodpen();
    }else if(__res == default_type::key){
        this->methodwrite();
    }


    bool ok;
    if(this->m_path.isEmpty())
    {
        /* asks where you want to save the new one */
        qfilechoose filec(this);
        ok = filec.filechoose(&m_path);
        if (!ok){
            return;
        }

        /* if the file already exists we want to delete all the copybook already exist */
        if(QFile::exists(m_path)){
            QFile::remove(m_path);
        }
    }

    QString namecopybook = QInputDialog::getText(this, tr("Get text"),
                                                 tr("Title: "), QLineEdit::Normal,
                                                 "", &ok);

    if(!ok || namecopybook == "")
        return;

    if(this->m_indice.titolo.indexOf(namecopybook) != -1)
        return dialog_critic("There is a copybook that already has this title");


    if(!newcopybook_(this, namecopybook))
        return dialog_critic("We had a problem saving the copybook");

    this->ui->listWidgetSX->setEnabled(true);
    update_list_copybook(this);

}
