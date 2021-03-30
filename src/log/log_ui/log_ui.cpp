#include "log_ui.h"
#include "ui_log_ui.h"

log_ui::log_ui(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::log_ui)
{
    ui->setupUi(this);
}

log_ui::~log_ui()
{
    delete ui;
}
