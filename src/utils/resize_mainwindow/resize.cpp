#include "../../mainwindow.h"
#include "ui_mainwindow.h"

void MainWindow::resizeEvent(QResizeEvent *event){
    return event->accept();
    short int height = this->ui->listWidgetSX->height();
    this->ui->button_left_hide->setFixedHeight(height);
    this->ui->button_right_hide->setFixedHeight(height);
}

void MainWindow::on_button_left_hide_clicked(){
    ui->listWidgetSX->setHidden(true);
    ui->button_left_hide->setHidden(true);

    ui->button_right_hide->setHidden(false);
}

void MainWindow::on_button_right_hide_clicked(){
    ui->listWidgetSX->setHidden(false);
    ui->button_left_hide->setHidden(false);

    ui->button_right_hide->setHidden(true);
}
