#include "../mainwindow.h"
#include "dialog_sheet.h"
#include <QSettings>

/* it load the last configuration for the file */
static int load_last(){

}

void MainWindow::on_actionnew_sheet_triggered()
{
    QSettings settings(QString("config_sheet"), QSettings::IniFormat);
    QString someValue = settings.value("some/config/key", "default value if unset").toString(); // settings.value() returns QVariant


    dialog_sheet *dialog = new dialog_sheet;
    dialog->exec();
}
