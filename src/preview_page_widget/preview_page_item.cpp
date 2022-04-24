#include "preview_page_item.h"
#include "touch/datastruct/page.h"
#include "ui_preview_page_item.h"

constexpr not_used bool preview_item_debug = true;

constexpr int _width = 200;
constexpr int _height = page::getProportion() * _width;

preview_page_item::preview_page_item(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::preview_page_item)
{
    W_ASSERT(parent == NULL);
    ui->setupUi(this);
}

preview_page_item::~preview_page_item()
{
    delete ui;
}

void preview_page_item::draw(const page &page)
{
    WDebug(preview_item_debug, "preview_page_item::draw");
    this->_page = &page;
    update();

    this->setFixedHeight(_height);
    this->setFixedWidth(_width);
}

void preview_page_item::paintEvent(QPaintEvent *)
{
    QPainter painter(this);
    QPixmap pix(_width, _height);
    constexpr QRect target(0, 0, _width, _height);
    const QImage &img = _page->getImg();

    WDebug(preview_item_debug, "preview_page_item::paintEvent call" << qstr("H: %1 W: %2").arg(height()).arg(width()));

    pix.fill(Qt::white);
    painter.drawPixmap(target, pix);
    painter.drawImage(QRect(0, 0, _width, _height), img);
    //ui->label->setPixmap(pix);
    painter.end();
    //img.save("/home/giacomo/Scrivania/prova.png", "PNG");
}
