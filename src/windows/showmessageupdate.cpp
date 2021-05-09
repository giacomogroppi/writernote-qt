#include "showmessageupdate.h"
#include "ui_showmessageupdate.h"
#include "../utils/copy_text.h"
#include "mostra_finestra_i.h"
#include "../mainwindow.h"
#define URL_PROG "https://github.com/giacomogroppi/writernote-qt/releases"
#include "../../updater/launch_updater/launch_updater.h"

ShowMessageUpdate::ShowMessageUpdate(QWidget *parent,
                                     QString body,
                                     QString new_ver) :
    QDialog(parent),
    ui(new Ui::ShowMessageUpdate)
{
    ui->setupUi(this);

    ui->label->setText(body);
    ui->label_curr_ver_val->setText(VERSION_STRING);
    ui->label_new_ver_val->setText(new_ver);

#if !defined(unix)
    ui->pushButton_terminal_command->setHidden(true);
#endif

#if !(defined(WIN32) || defined(WIN64))
    ui->pushButton_update_now->setHidden(true);
#endif
}

ShowMessageUpdate::~ShowMessageUpdate()
{
    delete ui;
}

void ShowMessageUpdate::on_button_copy_url_clicked()
{
    copy_text(URL_PROG);
}

void ShowMessageUpdate::on_pushButton_close_clicked()
{
    this->close();
}

void ShowMessageUpdate::on_pushButton_terminal_command_clicked()
{
    copy_text("sudo snap refresh writernote");
}

void ShowMessageUpdate::on_pushButton_brow_clicked()
{
    mostra_finestra_i(URL_PROG);
}

void ShowMessageUpdate::on_pushButton_update_now_clicked()
{
    launch_updater::launch();
}
