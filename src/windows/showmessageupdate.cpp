#include "showmessageupdate.h"
#include "ui_showmessageupdate.h"
#include "../utils/copy_text.h"
#include "mostra_finestra_i.h"
#define URL_PROG "https://github.com/giacomogroppi/writernote-qt/releases"


ShowMessageUpdate::ShowMessageUpdate(QWidget *parent,
                                     QString body) :
    QDialog(parent),
    ui(new Ui::ShowMessageUpdate)
{
    ui->setupUi(this);

    ui->label->setText(body);


#if !defined(unix)
    ui->pushButton_terminal_command->setHidden(true);
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
