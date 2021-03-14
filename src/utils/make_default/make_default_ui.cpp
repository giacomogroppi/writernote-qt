#include "make_default_ui.h"
#include "ui_make_default_ui.h"

#define CLOSE(x)     x = true; \
    this->close();

make_default_ui::make_default_ui(QWidget *parent, QString title, QString text) :
    QDialog(parent),
    ui(new Ui::make_default_ui)
{
    ui->setupUi(this);

    ui->label->setText(text);
    this->setWindowTitle(title);
}

make_default_ui::~make_default_ui()
{
    if(alreadyDecide)
        emit no(false);

    delete ui;
}

void make_default_ui::on_button_no_clicked()
{
    emit no(ui->choise->isChecked());

    CLOSE(alreadyDecide)
}

void make_default_ui::on_button_yes_clicked()
{
    emit yes(ui->choise->isChecked());

    CLOSE(alreadyDecide)
}
