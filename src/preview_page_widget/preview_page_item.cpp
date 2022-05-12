#include "preview_page_item.h"
#include "preview_page_widget/preview_page_widget.h"
#include "qlabel.h"
#include "touch/datastruct/page.h"
#include "ui_preview_page_item.h"
#include "touch/tabletcanvas.h"
#include "utils/utils.h"

constexpr not_used bool preview_item_debug = false;

constexpr auto pos_img = ":image/images/preview_page_item_image1.png";

constexpr int _widthImg = 200;
constexpr int _heightImg = page::getProportion() * _widthImg;

constexpr int realWidth = _widthImg;
constexpr int realHeight = _heightImg + 40;

preview_page_item::preview_page_item(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::preview_page_item)
{
    W_ASSERT(parent == NULL);
    ui->setupUi(this);
    _lab = new QLabel(this);
    this->layout()->addWidget(_lab);

    this->setMinimumSize(QSize(realWidth, realHeight));
    setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
}

preview_page_item::~preview_page_item()
{
    delete ui;
}

void preview_page_item::draw(const page &page, cbool selected)
{
    WDebug(preview_item_debug, "preview_page_item::draw call");

    this->_page = &page;
    this->_selected = selected;
    this->_index = page.getCount() - 1;


    // draw the image and the sheet in the pixmap
    {
        this->_pix = QPixmap(_widthImg, _heightImg);
        _pix.fill(Qt::white);
        this->paint(_pix);

        QPainter painter(&_pix);
        painter.drawImage(QRect(0, 0, _widthImg, _heightImg), _page->getImg());
        painter.end();
    }

    this->setMinimumSize(QSize(realWidth, realHeight));
    this->setFixedHeight(realHeight);
    update();
    W_ASSERT(this->size() == QSize(realWidth, realHeight));
}

QSize preview_page_item::get_size()
{
    return QSize(realWidth, realHeight);
}

void preview_page_item::paint(QPixmap &pix)
{
    constexpr double delta = _widthImg / page::getWidth();
    const QPointF pointZero(0., - page::getHeight() * delta * double(_page->getCount() - 1));
    QPainter painter(&pix);
    Define_PEN(pen);

    drawUtils::loadSingleSheet(painter, *_page, 1., delta, pen, pointZero);
    painter.end();
}

constexpr QRect target_prop(173, 251, 30, 30);

void preview_page_item::paintEvent(QPaintEvent *)
{
    constexpr QRect target(0, 0, _widthImg, _heightImg);
    constexpr int deltaImgTouchWidth = 27;
    constexpr int deltaImgTouchHeight = 31;
    constexpr int height = 30;

    double delta;
    QPainter painter(this);

    QImage touchImg(pos_img);
    Define_PEN(pen);

    if(_index < 0)
        return;

    /*if(_page->getCount() % 2 == 0){
        pix.fill(Qt::blue);
    }else{
        pix.fill(Qt::red);
    }*/

    painter.drawPixmap(target, _pix);

    if(unlikely(_selected)){
        pen.setWidth(1);
        pen.setColor(Qt::blue);
        painter.setPen(pen);

        painter.drawRect(QRect(0, 0, _widthImg - 1, _heightImg - 1));
    }


    {
        const auto widthNew     = _widthImg -  deltaImgTouchWidth;
        const auto heightNew    = _heightImg - deltaImgTouchHeight;

        painter.drawImage(target_prop, touchImg, touchImg.rect());
    }

    painter.drawText(QPointF(3., _heightImg + 20.), QString::number(this->_index + 1));

    painter.end();
}

bool preview_page_item::event(QEvent *event)
{
    constexpr bool debugEvent = true;
    constexpr int delta = 100;

    static unsigned long last;
    const auto current = utils::get_time_since_epoch();

    if(event->type() == QEvent::MouseButtonPress){
        last = current;
    }
    else if(event->type() == QEvent::MouseButtonRelease){
        const auto pos = static_cast<QMouseEvent *>(event)->pos();

        if(target_prop.contains(pos)){
            WDebug(debugEvent, "preview_page_item::event show prop");
            auto *prop = preview_page_widget::get_list();
            const auto GlobalPos = static_cast<QMouseEvent *>(event)->globalPos();
            prop->Show(GlobalPos, _index);
        }
        else if(last + delta >= current){
            WDebug(debugEvent, "preview_page_item::event click done");

            emit clickUser(this);

        }
    }

    return QWidget::event(event);
}
