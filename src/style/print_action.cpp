#include "../mainwindow.h"
#include "ui_mainwindow.h"

#include <QPrintDialog>

/* stampa */
void MainWindow::on_actionPrint_triggered()
{
    QPrintDialog *dialogprint = new QPrintDialog;

    /*if (dialogprint->exec())
        this->ui->textEdit->print(dialogprint->printer());*/
}
