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

void preview_page_container::draw(int index)
{
    const datastruct *data = this->_main->getCurrentDoc()->datatouch;
    auto &ref = _item_show[index];
    ref->draw(data->at(index));
}

void preview_page_container::draw(const QVector<int> &pos)
{
    int i = pos.length();

    for(i--; i >= 0; i++){
        this->draw( pos.at(i) );
    }
}

void preview_page_container::drawAll()
{
    int i = this->_item_show.length();
    for(i --; i >= 0; i--){
        this->draw(i);
    }
}

void preview_page_container::pageMove()
{
    const auto *data = _main->getCurrentDoc()->datatouch;
    int index = data->getFirstPageVisible();
    if(!this->_item_show.at(index)->isVisible()){
        // do translation
    }

}

void preview_page_container::newPage()
{
    if(!this->_item_not_show.isEmpty()){
        this->_item_show.append(
                    _item_not_show.takeAt(0)
                    );
    }

    this->drawAll();
    this->pageMove();
}

void preview_page_container::changeDocument()
{
    this->appendNecessary();

    this->drawAll();
    this->pageMove();
}

void preview_page_container::appendNecessary()
{
    W_ASSERT(this->_item_show.length() == 0);
    int needLen = _main->getCurrentDoc()->datatouch->lengthPage();
    int i;

    if(_item_not_show.length() < needLen){
        for(i = needLen - _item_not_show.length(); i > 0; i--){
            preview_page_item *item;

            WNew(item, preview_page_item, (NULL));

            _item_show.append(
                            item
                        );
        }

        this->_item_show.append(_item_not_show);
    }else{
        for(i = 0; i < needLen; i++){
            _item_show.append(_item_not_show.takeAt(0));
        }
    }

    W_ASSERT(this->_item_show.length() == needLen);
}
