#include "preview_page_container.h"
#include "qlabel.h"
#include "ui_preview_page_container.h"
#include "mainwindow.h"

preview_page_container::preview_page_container(QWidget *parent, MainWindow *mainWindow) :
    QWidget(parent),
    ui(new Ui::preview_page_container)
{
    ui->setupUi(this);
    _main = mainWindow;
    this->layout()->setAlignment(Qt::AlignTop);
    //this->layout()->setSpacing(10);
    this->layout()->setMargin(10);
    this->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
}

preview_page_container::~preview_page_container()
{
    preview_page_item *item;

    while(_item_not_show.length() > 0){
        item = _item_not_show.takeAt(0);
        WDelete(item);
    }

    while(_item_show.length() > 0){
        item = _item_show.takeAt(0);

        this->layout()->removeWidget(item);
        WDelete(item);
    }

    delete ui;
}

void preview_page_container::draw(int index)
{
    const datastruct *data = this->_main->getCurrentDoc()->datatouch;
    static int last;
    const int selected = data->getFirstPageVisible();
    auto *ref = at_show(index);

    if(unlikely(last != selected)){
        last = selected;
        return this->drawAll();
    }

    ref->draw(data->at(index), selected == index);
}

void preview_page_container::clickUser(void *_this)
{
    const auto *item = (preview_page_item *)_this;
    int index = this->get_index(item);
    auto *data = _main->getCurrentDoc()->datatouch;

    W_ASSERT(index >= 0);

    data->moveToPage(index);

    _main->_canvas->call_update();
    emit changePage(index);
}

void preview_page_container::draw(const QVector<int> &pos)
{
    int i = pos.length();

    for(i--; i >= 0; i--){
        const auto index = pos.at(i);
        this->draw(index);

        this->at_show(index)->setVisible(true);
    }

    WDebug(false, "preview_page_container::draw" << this->layout()->count() << at_show(0)->isVisible());
}

void preview_page_container::drawAll()
{
    int i, len = this->_item_show.length();
    for(i = 0; i < len; i++){
        this->draw(i);
    }
}

void preview_page_container::newPage()
{
    preview_page_item *item;

    if(!this->_item_not_show.isEmpty()){
        item = _item_not_show.takeAt(0);
        appendToVisible( item );
    }else{
        WNew(item, preview_page_item, (NULL));

        appendToVisible( item );
    }

    this->layout()->addWidget(item);
    item->setVisible(true);
    this->drawAll();

    {
        const auto size = preview_page_item::get_size();
        this->setMinimumSize(size.width(), size.height() * _item_show.length());
    }
}

void preview_page_container::changeDocument()
{
    for(preview_page_item *item : qAsConst(_item_show)){
        item->setInvalid();
        item->hide();
        this->layout()->removeWidget(item);
    }

    W_ASSERT(this->layout()->count() == 0);

    _item_not_show.append(_item_show);
    _item_show.clear();

    this->appendNecessary();

    this->drawAll();
    W_ASSERT(this->_item_show.length() == _main->getCurrentDoc()->datatouch->lengthPage());
}

int preview_page_container::get_index(const preview_page_item *item)
{
    return _item_show.indexOf((preview_page_item *)item);
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

            _item_show.append( item );
        }

        appendToVisible(_item_not_show);
        _item_not_show.clear();
    }else{
        for(i = 0; i < needLen; i++){
            appendToVisible(_item_not_show.takeAt(0));
        }
    }

    for(i = _item_show.length() - 1; i >= 0; i--){
        auto *item = at_show(i);
        layout()->addWidget(item);
        item->setVisible(true);
    }

    for(auto *item : qAsConst(_item_show)){
        item->setInvalid();
    }

    W_ASSERT(this->_item_show.length() == layout()->count());
    W_ASSERT(this->_item_show.length() == needLen);
}
