#include "../mainwindow.h"
#include "ui_mainwindow.h"

#ifndef IOS

#include <QPrintDialog>
#include <QDialog>
#include <QPrinter>

void MainWindow::on_actionPrint_triggered()
{
#ifdef ANDROID
#elif IOS
#else
    QPrinter printer;
    QPrintDialog dialogprint(&printer);

    if (dialogprint.exec() == QDialog::Accepted)
        this->ui->textEdit->print(&printer);
#endif
}

#endif
