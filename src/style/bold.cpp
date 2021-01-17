#ifndef BOLD_CPP
#define BOLD_CPP

#include "../mainwindow.h"
#include <QFont>
#include "ui_mainwindow.h"

void MainWindow::on_boldbotton_clicked()
{
    if (this->ui->textEdit->fontWeight() == QFont::Bold)
        this->ui->textEdit->setFontWeight(QFont::Normal);
    else
        this->ui->textEdit->setFontWeight(QFont::Bold);
}


#endif
