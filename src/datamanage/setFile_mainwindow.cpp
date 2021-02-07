#include "../mainwindow.h"

void MainWindow::setFile(char **argv){
    this->on_actionOpen_triggered(argv[1]);
}
