#include "ui_mainwindow.h"
#include "mainwindow.h"

void MainWindow::on_actionEnable_redo_undo_triggered()
{
    if(this->enableredoundo)
        ui->actionEnable_redo_undo->setText(REDO_UNDO_DISABLE);
    else
        ui->actionEnable_redo_undo->setText(REDO_UNDO_ENABLE);

    enableredoundo = !enableredoundo;

    this->m_canvas->enableredoundu = enableredoundo;
}
