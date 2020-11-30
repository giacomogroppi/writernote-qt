#ifndef DISPLAY_ERROR_MESSAGE_CPP
#define DISPLAY_ERROR_MESSAGE_CPP

#include "../mainwindow.h"
#include "ui_mainwindow.h"

void MainWindow::displayErrorMessage()
{
    ui->statusBar->showMessage(m_audioRecorder->errorString());
}

#endif //DISPLAY_ERROR_MESSAGE_CPP
