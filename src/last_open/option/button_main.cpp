#include "../../mainwindow.h"
#include "option_last_open_ui.h"
#include "../last_open.h"

#define mac_ex(parent) \
    option_last_open_ui opt(parent); \
    opt.exec();

void MainWindow::on_actionLast_open_file_triggered()
{
    mac_ex(nullptr);

}

void last_open::on_option_button_clicked()
{
    mac_ex(nullptr);
}
