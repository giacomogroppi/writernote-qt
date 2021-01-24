#include "../mainwindow.h"

#include <QSettings>

void MainWindow::on_actionnew_sheet_triggered()
{
    QSettings settings(QString("config_sheet"), QSettings::IniFormat);
    QString someValue = settings.value("some/config/key", "default value if unset").toString(); // settings.value() returns QVariant

}
