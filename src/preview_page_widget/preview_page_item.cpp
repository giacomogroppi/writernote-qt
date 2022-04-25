#include "preview_page_item.h"
#include "qlabel.h"
#include "touch/datastruct/page.h"
#include "ui_preview_page_item.h"
#include "touch/tabletcanvas.h"

constexpr not_used bool preview_item_debug = true;

constexpr int _width = 200;
constexpr int _height = page::getProportion() * _width;

preview_page_item::preview_page_item(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::preview_page_item)
{
    W_ASSERT(parent == NULL);
    ui->setupUi(this);
    _lab = new QLabel(this);
    this->layout()->addWidget(_lab);

    this->setMinimumSize(QSize(_width, _height));
    setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
}

preview_page_item::~preview_page_item()
{
    delete ui;
}

void preview_page_item::draw(const page &page)
{
    WDebug(preview_item_debug, "preview_page_item::draw call" << this->sizePolicy());

    this->_page = &page;

    this->setMinimumSize(QSize(_width, _height));
    this->setFixedHeight(_height);
    update();
}

QSize preview_page_item::get_size() const
{
    return QSize(_width, _height);
}

void preview_page_item::paint(QPixmap &pix)
{
    constexpr double delta = _width / page::getWidth();
    const QPointF pointZero(0., - page::getHeight() * delta * double(_page->getCount() - 1));
    QPainter painter(&pix);
    Define_PEN(pen);

    drawUtils::loadSingleSheet(painter, *_page, 1., delta, pen, pointZero);
    painter.end();
}

void preview_page_item::paintEvent(QPaintEvent *)
{
    QPainter painter(this);
    QPixmap pix(_width, _height);
    constexpr QRect target(0, 0, _width, _height);
    const QImage &img = _page->getImg();

    WDebug(preview_item_debug, "preview_page_item::paintEvent call" << qstr("H: %1 W: %2").arg(height()).arg(width()));

    if(_page->getCount() % 2 == 0){
        pix.fill(Qt::blue);
    }else{
        pix.fill(Qt::red);
    }

    pix.fill(Qt::white);

    this->paint(pix);

    painter.drawPixmap(target, pix);
    painter.drawImage(QRect(0, 0, _width, _height), img);

    painter.end();
}
