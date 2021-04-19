#include "ui_scroll.h"
#include "ui_ui_scroll.h"

ui_scroll::ui_scroll(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ui_scroll)
{
    ui->setupUi(this);
}

ui_scroll::~ui_scroll()
{
    delete ui;
}
