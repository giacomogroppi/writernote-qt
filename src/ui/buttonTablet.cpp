#include "ui_mainwindow.h"
#include "../mainwindow.h"
#include "controlluibutton.h"

void MainWindow::on_buttonRecentFile_clicked()
{
    this->on_actionRecent_file_triggered();
}

void MainWindow::on_actionChange_visual_triggered()
{
    this->m_controllUi->change();
}

void MainWindow::on_buttonChangeVisual_clicked()
{
    this->m_controllUi->change();
}

void MainWindow::on_buttonPen_clicked()
{
    this->on_actionpen_triggered();
}
