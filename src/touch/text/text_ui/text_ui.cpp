#include "text_ui.h"
#include "ui_text_ui.h"

text_ui::text_ui(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::text_ui)
{
    ui->setupUi(this);
}

text_ui::~text_ui()
{
    delete ui;
}

