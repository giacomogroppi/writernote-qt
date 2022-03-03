#include "mainwindow.h"
#include "dialog_sheet.h"
#include <QSettings>

void MainWindow::on_actionnew_sheet_triggered()
{
    dialog_sheet dialog(nullptr, _canvas->_sheet);
    dialog.exec();
}
