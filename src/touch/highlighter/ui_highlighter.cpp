#include "ui_highlighter.h"
#include "ui_ui_highlighter.h"

ui_highlighter::ui_highlighter(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ui_highlighter)
{
    ui->setupUi(this);
}

ui_highlighter::~ui_highlighter()
{
    delete ui;
}
