#include "preview_page_container.h"
#include "ui_preview_page_container.h"

preview_page_container::preview_page_container(QWidget *parent, MainWindow *mainWindow) :
    QWidget(parent),
    ui(new Ui::preview_page_container)
{
    ui->setupUi(this);
}

preview_page_container::~preview_page_container()
{
    while(_item_not_show.length()){
        WFree(_item_not_show.takeAt(0));
    }

    while(_item_show.length()){
        WFree(_item_show.takeAt(0));
    }

    delete ui;
}
