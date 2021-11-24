#include "../mainwindow.h"
#include "../../updater/launch_updater/launch_updater.h"

void MainWindow::on_actionUpdate_writernote_triggered()
{
#if defined(WIN32) || defined(WIN64)
    launch_updater::launch();
#endif
}
