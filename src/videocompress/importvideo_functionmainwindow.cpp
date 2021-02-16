#include "../mainwindow.h"
#include "ui_mainwindow.h"

#include "videocompress_ui.h"

void MainWindow::on_actioncompress_video_triggered()
{
    videocompress_ui *videocompress = new videocompress_ui(nullptr, this);

    videocompress->exec();
}
