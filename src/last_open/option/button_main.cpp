#include "../../mainwindow.h"
#include "option_last_open_ui.h"
#include "../last_open.h"



void MainWindow::on_actionLast_open_file_triggered()
{


}

void last_open::on_option_button_clicked()
{
    option_last_open_ui opt(nullptr);
    opt.exec();
}
