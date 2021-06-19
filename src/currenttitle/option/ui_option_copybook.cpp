#include "ui_option_copybook.h"
#include "ui_ui_option_copybook.h"

ui_option_copybook::ui_option_copybook(QWidget *parent,
                                       QList<modality> *list) :
    QDialog(parent),
    ui(new Ui::ui_option_copybook)
{
    ui->setupUi(this);
}

ui_option_copybook::~ui_option_copybook()
{
    delete ui;
}
