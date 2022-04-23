#include "preview_page_item.h"
#include "ui_preview_page_item.h"

preview_page_item::preview_page_item(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::preview_page_item)
{
    ui->setupUi(this);
}

preview_page_item::~preview_page_item()
{
    delete ui;
}
