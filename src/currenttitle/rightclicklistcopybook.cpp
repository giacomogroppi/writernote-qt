#include "../mainwindow.h"
#include "ui_mainwindow.h"


#include <QMenu>
#include "../utils/dialog_critic/dialog_critic.h"
#include <QInputDialog>
#include "redolist.h"
#include <QString>

#include <QContextMenuEvent>

#include "deletecopybook.h"
#include "audiototext.h"
#include "renamefile.h"

#include "../utils/areyousure/areyousure.h"

void MainWindow::deletecopybook_f(){
    if(!this->ui->listWidgetSX->currentItem()->isSelected())
        return redolist(this);

    f_deletecopybook(this, this->ui->listWidgetSX->currentItem()->text().toUtf8().constData());
}

void MainWindow::compressvideo_f(){
    if(!this->ui->listWidgetSX->currentItem()->isSelected())
        return redolist(this);

    if(this->ui->listWidgetSX->currentItem()->text() != this->m_currentTitle){
            if(!areyousure("Warning", "For compress video you need to change the current copybook\nDo you want to continue?"))
                return;

        this->on_listWidgetSX_itemDoubleClicked(this->ui->listWidgetSX->currentItem());
    }

    this->on_actioncompress_video_triggered();
}

void MainWindow::renamefile_f(){
    if(!this->ui->listWidgetSX->currentItem()->isSelected())
        return redolist(this);

    renamefile(this, this->ui->listWidgetSX->currentItem()->text().toUtf8().constData());

}


void MainWindow::audiototext_f(){
    messaggio_utente("This option is not implemented in the current version");
    if(!this->ui->listWidgetSX->currentItem()->isSelected()){
        messaggio_utente("This version of writernote is not yet capable of translating audio to text");
        return redolist(this);
    }
}

#include "deleteaudio.h"
void MainWindow::deleteaudio_f(){
    if(!this->ui->listWidgetSX->currentItem()->isSelected())
        return redolist(this);
    deleteaudio(this, this->ui->listWidgetSX->currentItem());
}

/* funzione che gestisce il click destro sulla lista dei copybook */
void MainWindow::contextMenuEvent(QContextMenuEvent *event){
    QAction *deletecopybook_action;
    QAction *renamecopybook_action;
    QAction *audiototext_action;
    QAction *compressvideo_action;
    QAction *deleteaudio_action;

    audiototext_action = new QAction(tr("audio -> text"), this);
    audiototext_action->setStatusTip(tr("convert the audio into text"
                            "clipboard"));
    connect(audiototext_action, &QAction::triggered, this, &MainWindow::audiototext_f);

    compressvideo_action = new QAction(tr("compress video"), this);
    compressvideo_action->setStatusTip(tr("compress the video you imported with ffmpeg"
                            "clipboard"));

    connect(compressvideo_action, &QAction::triggered, this, &MainWindow::compressvideo_f);

    deletecopybook_action = new QAction(tr("delete"), this);
    deletecopybook_action->setStatusTip(tr("delete the copybook"
                            "clipboard"));

    connect(deletecopybook_action, &QAction::triggered, this, &MainWindow::deletecopybook_f);

    renamecopybook_action = new QAction(tr("rename the copybook"), this);
    renamecopybook_action->setStatusTip(tr("rename the copybook"
                            "clipboard"));

    connect(renamecopybook_action, &QAction::triggered, this, &MainWindow::renamefile_f);

    deleteaudio_action = new QAction(tr("delete audio"), this);
    deleteaudio_action->setStatusTip(tr("delete the audio of the copybook"
                            "clipboard"));

    connect(deleteaudio_action, &QAction::triggered, this, &MainWindow::deleteaudio_f);

    QMenu menu(this);
    menu.addAction(deletecopybook_action);
    menu.addAction(audiototext_action);
    menu.addAction(deleteaudio_action);
    menu.addAction(compressvideo_action);
    menu.addAction(renamecopybook_action);
    menu.exec(event->globalPos());
}

