#include "preview_page_item.h"
#include "touch/datastruct/page.h"
#include "ui_preview_page_item.h"

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
    this->_page = &page;
    update();
}

void preview_page_item::paintEvent(QPaintEvent *)
{
    constexpr int width = 200;
    constexpr int height = page::getProportion() * width;
    QPainter painter;
    const QImage &img = _page->getImg();

    W_ASSERT(painter.begin(ui->label));

    painter.drawImage(QRect(0, 0, width, height), img);

    painter.end();
}
