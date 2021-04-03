#include "default_option_ui.h"
#include "ui_default_option_ui.h"

default_option_ui::default_option_ui(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::default_option_ui)
{
    ui->setupUi(this);
}

default_option_ui::~default_option_ui()
{
    delete ui;
}
