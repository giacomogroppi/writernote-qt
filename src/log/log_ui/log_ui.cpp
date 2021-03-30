#include "log_ui.h"
#include "ui_log_ui.h"

log_ui::log_ui(QWidget *parent, QString * pos_log) :
    QWidget(parent),
    ui(new Ui::log_ui)
{
    ui->setupUi(this);

    this->pos_log = pos_log;

}

log_ui::~log_ui()
{
    delete ui;
}

void log_ui::closeEvent(QCloseEvent *event)
{
    /* we don't want to close actualy */
    this->close();

    event->ignore();
}
