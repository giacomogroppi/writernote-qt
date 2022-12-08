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
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
    this->layout()->setMargin(10);
#else
    this->layout()->setSpacing(10);
#endif
    this->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
}

preview_page_container::~preview_page_container()
{
    preview_page_item *item;

    while(_item_not_show.length() > 0){
        item = _item_not_show.takeAt(0);
        WDelete(item);
    }

    this->removeAllItemFromLayout();

    while(_item_show.length() > 0){
        item = _item_show.takeAt(0);


        WDelete(item);
    }

    delete ui;
}

void preview_page_container::draw(int index)
{
    const datastruct *data = this->_main->getCurrentDoc();
    static int last;
    const int selected = data->getFirstPageVisible();
    auto *ref = at_show(index);

    if(un(last != selected)){
        last = selected;
        return this->drawAll();
    }

    ref->draw(data->at(index), selected == index);
}

void preview_page_container::clickUser(void *_this)
{
    const auto *item = (preview_page_item *)_this;
    int index = this->get_index(item);
    auto *data = _main->getCurrentDoc();

    W_ASSERT(index >= 0);

    data->moveToPage(index);

    this->drawAll();
    _main->_canvas->updatePageCount();
    _main->_canvas->call_update();
}

void preview_page_container::draw(const QVector<int> &pos)
{
    int i = pos.length();

    for(i--; i >= 0; i--){
        const auto index = pos.at(i);

        if(index < 0){
            return this->drawAll();
        }

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
    const datastruct *data = _main->getCurrentDoc();

    if(!this->_item_not_show.isEmpty()){
        item = _item_not_show.takeAt(0);
        appendToVisible( item );
    }else{
        WNew(item, preview_page_item, (NULL));

        appendToVisible( item );
    }

    if(un(data->lengthPage() != this->_item_show.length()))
        return this->changeDocument();

    this->layout()->addWidget(item);
    item->setVisible(true);
    this->drawAll();
    this->Resize();
}

void preview_page_container::changeDocument()
{
    const not_used datastruct *data = _main->getCurrentDoc();
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
    this->Resize();

    W_ASSERT(this->_item_show.length() == _main->getCurrentDoc()->lengthPage());
    W_ASSERT(this->layout()->count() == _item_show.length());

#ifdef DEBUGINFO
    int i, len = this->_item_show.length();
    for(i = 0; i < len; i++){
        const auto *item = at_show(i);

        W_ASSERT(layout()->itemAt(i)->widget() == item);
        if(item->_page->getCount() != i + 1){
            int i, len;

            len = data->lengthPage();
            for(i = 0; i < len; i++){
                item = at_show(i);
                qDebug() << "preview_page_container::changeDocument assert" << item->_page->getCount() << data->at(i).getCount() << "i:" << i;
            }

            std::abort();
        }
    }
#endif
}

void preview_page_container::Resize()
{
    const auto size = preview_page_item::get_size();
    this->setMinimumSize(size.width(), size.height() * _item_show.length());
}

void preview_page_container::disableAll()
{
    this->setAllDisable();
    this->removeAllItemFromLayout();
    this->_item_not_show.append(this->_item_show);
    _item_show.clear();
}

void preview_page_container::removeAllItemFromLayout()
{
    for(auto *item : qAsConst(_item_show)){
        this->layout()->removeWidget(item);
    }
}

int preview_page_container::get_index(const preview_page_item *item)
{
    return _item_show.indexOf((preview_page_item *)item);
}

void preview_page_container::setAllDisable()
{
    for(auto *item : qAsConst(_item_show)){
        item->setInvalid();
    }
}

void preview_page_container::appendNecessary()
{
    W_ASSERT(this->_item_show.length() == 0);
    int needLen = _main->getCurrentDoc()->lengthPage();
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

    for(i = 0; i < _item_show.length(); i++){
        auto *item = at_show(i);
        layout()->addWidget(item);
        item->setVisible(true);
    }

    this->setAllDisable();

    W_ASSERT(this->_item_show.length() == layout()->count());
    W_ASSERT(this->_item_show.length() == needLen);
}
