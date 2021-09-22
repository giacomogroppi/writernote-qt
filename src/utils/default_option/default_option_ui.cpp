#include "default_option_ui.h"
#include "ui_default_option_ui.h"

default_option_ui::default_option_ui(QWidget *parent,
                                     QString first_button,
                                     QString second_button,
                                     QString title,
                                     QString text, const uchar First) :
    QDialog(parent),
    ui(new Ui::default_option_ui)
{
    ui->setupUi(this);

    ui->first_text->setCheckable(true);
    ui->second_text->setCheckable(true);

    ui->first_text->setText(first_button);
    ui->second_text->setText(second_button);
    this->setWindowTitle(title);

    ui->first_text->setCheckable(true);
    ui->second_text->setCheckable(true);

    ui->text->setText(text);

    res = First;

    this->updateStatus();
}

default_option_ui::~default_option_ui()
{
    delete ui;
}

void default_option_ui::setChecked(default_option_ui::e_result val)
{
    if(val == default_option_ui::first){
        res = 1;
    }
    else if(val == default_option_ui::second){
        res = 2;
    }
    else if(val == default_option_ui::none){
        res = -1;
    }

    this->updateStatus();
}

void default_option_ui::on_button_cancel_clicked()
{
    this->close();
}

void default_option_ui::on_button_ok_clicked()
{
    if(res == 1)
        emit result(e_result::first);
    else if(res == 2)
        emit result(e_result::second);
    else
        emit result(e_result::error);
}

void default_option_ui::on_second_text_clicked()
{
    res = 2;
    updateStatus();
}

void default_option_ui::on_first_text_clicked()
{
    res = 1;
    updateStatus();
}

void default_option_ui::updateStatus(){
    ui->first_text->setChecked(res == 1);
    ui->second_text->setChecked(res == 2);
}
