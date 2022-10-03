#include "mainwindow.h"
#include "updater/launch_updater/launch_updater.h"
#include "log/log_ui/log_ui.h"

void MainWindow::on_actionUpdate_writernote_triggered()
{
#if defined(WIN32) || defined(WIN64)
    launch_updater::launch();

#else

    NAME_LOG_EXT->write("it is not available for systems other than windows", log_ui::possible_bug);
#endif
}
