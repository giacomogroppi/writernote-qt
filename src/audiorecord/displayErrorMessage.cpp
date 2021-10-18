#include "../mainwindow.h"
#include "ui_mainwindow.h"

void MainWindow::displayErrorMessage()
{
    ui->statusBar->showMessage(m_audioRecorder->errorString());
}

