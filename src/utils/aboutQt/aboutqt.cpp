#include "aboutqt.h"
#include <QMessageBox>

#include "mainwindow.h"

void aboutQt()
{
    QMessageBox::aboutQt(nullptr);
}

void MainWindow::on_actionAbout_Qt_triggered(){
    aboutQt();
}
