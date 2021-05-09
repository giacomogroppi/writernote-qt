#include "../mainwindow.h"
#include "../../updater/launch_updater/launch_updater.h"

void MainWindow::on_actionUpdate_writernote_triggered()
{
    launch_updater::launch();
}
