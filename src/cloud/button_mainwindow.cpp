#include "../mainwindow.h"
#include "cloud_ui.h"

void MainWindow::on_actioncloud_triggered()
{
#ifdef CLOUD
    cloud_ui cloud(nullptr, m_cloud);
    cloud.exec();
#endif
}
