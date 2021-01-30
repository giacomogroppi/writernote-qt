#include "ui_mainwindow.h"
#include "../mainwindow.h"

void MainWindow::on_actionEnable_redo_undo_triggered()
{
    if(this->enableredoundu)
        ui->actionEnable_redo_undo->setText("Disable redo undo");
    else
        ui->actionEnable_redo_undo->setText("Enable redo undo");

    enableredoundu = !enableredoundu;

    this->m_canvas->enableredoundu = enableredoundu;
}
