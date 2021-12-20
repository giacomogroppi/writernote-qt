#include "mainwindow.h"

#ifdef CLOUD
#include "cloud_ui.h"
#endif

void MainWindow::on_actioncloud_triggered()
{
#ifdef CLOUD
    cloud_ui cloud(nullptr, m_cloud);
    cloud.exec();
#endif
}
