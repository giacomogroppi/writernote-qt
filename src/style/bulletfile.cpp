#ifndef BULLET_FILE_CPP
#define BULLET_FILE_CPP

#include "../mainwindow.h"

void MainWindow::on_listbotton_clicked()
{
    auto cursor = this->ui->textEdit->textCursor();
    cursor.insertList(QTextListFormat::ListDisc);
}

#endif
