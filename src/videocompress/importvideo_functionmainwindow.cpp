#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "videocompress_ui.h"

void MainWindow::on_actioncompress_video_triggered(){
    videocompress_ui videocompress(nullptr, this);

    videocompress.exec();
}
