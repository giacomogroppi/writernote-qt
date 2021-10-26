#include "property.h"
#include "ui_property.h"

property::property(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::property)
{
    ui->setupUi(this);

    this->setWindowFlags(Qt::Window | Qt::FramelessWindowHint);
}

property::~property()
{
    delete ui;
}

QRect property::getPos(const QPoint &topLeft, const QPoint &bottomRight)
{
    QRect rect;
    const QPointF size(this->width(), this->height());
    const QPointF pointMiddle;

    rect.setHeight(20 + size.y());
    return rect;
}

void property::Show(const QRect &rect)
{
    this->show();
    this->setGeometry(rect);
}

void property::Hide()
{
    this->hide();
}
