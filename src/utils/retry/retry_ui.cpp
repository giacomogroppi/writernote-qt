#include "retry_ui.h"
#include "ui_retry_ui.h"

retry_ui::retry_ui(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::retry_ui)
{
    ui->setupUi(this);
}

retry_ui::~retry_ui()
{
    delete ui;
}
