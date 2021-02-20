#include "text_widgets.h"
#include "ui_text_widgets.h"

text_widgets::text_widgets(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::text_widgets)
{
    ui->setupUi(this);

}

text_widgets::~text_widgets()
{
    delete ui;
}
