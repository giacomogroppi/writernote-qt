#include "ui_mainwindow.h"
#include "mainwindow.h"

void MainWindow::on_actionEnable_redo_undo_triggered()
{
    redoundo *_undo = _canvas->_redoundo;

    if(_undo){
        WDelete(_canvas->_redoundo);
        _canvas->_redoundo = NULL;
        ui->actionEnable_redo_undo->setText(QApplication::tr(REDO_UNDO_ENABLE));
    }else{
        WNew(_canvas->_redoundo, redoundo, (_canvas));
        ui->actionEnable_redo_undo->setText(QApplication::tr(REDO_UNDO_DISABLE));
    }
}
