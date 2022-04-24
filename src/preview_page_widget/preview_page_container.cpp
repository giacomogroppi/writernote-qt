#include "preview_page_container.h"
#include "ui_preview_page_container.h"
#include "mainwindow.h"

preview_page_container::preview_page_container(QWidget *parent, MainWindow *mainWindow) :
    QWidget(parent),
    ui(new Ui::preview_page_container)
{
    ui->setupUi(this);
    _main = mainWindow;
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

void preview_page_container::updatePage()
{

}

void preview_page_container::draw(const QVector<int> &pos)
{
    int i = pos.length() - 1;
    const auto &data = *this->_main->getCurrentDoc()->datatouch;

    for(; i >= 0; i++){
        auto &ref = _item_show[i];
        ref->draw(data.at(pos.at(i)));
    }
}