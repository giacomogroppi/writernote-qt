#include "../mainwindow.h"
#include "ui_mainwindow.h"

void MainWindow::on_actionAuto_Save_triggered()
{
    if(this->autosave)
        this->ui->actionAuto_Save->setText("Disable auto save");
    else
        this->ui->actionAuto_Save->setText("Enable auto save");

    this->autosave = !autosave;
}
