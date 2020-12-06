#include "../mainwindow.h"
#include "ui_mainwindow.h"

#include <QPrintDialog>
#include <QDialog>
#include <QPrinter>

/* stampa */
void MainWindow::on_actionPrint_triggered()
{
    QPrinter *printer = new QPrinter;
    QPrintDialog *dialogprint = new QPrintDialog(printer);

    if (dialogprint->exec() == QDialog::Accepted)
        this->ui->textEdit->print(printer);
}
