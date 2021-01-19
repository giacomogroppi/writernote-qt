#include "../mainwindow.h"

void MainWindow::setFile(int argc, char **argv){
    this->on_actionOpen_triggered(argv[1]);
}
