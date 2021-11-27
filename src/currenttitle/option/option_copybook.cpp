#include "option_copybook.h"
#include "ui_option_copybook.h"

option_copybook::option_copybook(QWidget *parent,
                                       QList<modality> *list) :
    QDialog(parent),
    ui(new Ui::option_copybook)
{
    Q_UNUSED(list);
    ui->setupUi(this);
}

option_copybook::~option_copybook()
{
    delete ui;
}
