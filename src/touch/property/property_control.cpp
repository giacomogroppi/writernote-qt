#include "property_control.h"
#include "ui_property_control.h"

property_control::property_control(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::property_control)
{
    ui->setupUi(this);

    this->setWindowFlags(Qt::Window | Qt::FramelessWindowHint);
}

property_control::~property_control()
{
    delete ui;
}

QRect property_control::getPos(const QPoint &topLeft, const QPoint &bottomRight)
{
    QRect rect;
    const QPoint size(this->width(), this->height());
    const QPoint pointMiddle((topLeft.x()+bottomRight.x())/2.0, (topLeft.y() + bottomRight.y())/2.0);
    const QPoint halfSize = size/2;

    rect = QRect(QPoint(
                     (pointMiddle.x() + halfSize.x()),
                     (pointMiddle.y() + halfSize.y())),
                 QPoint(
                     (pointMiddle.x() - halfSize.x()),
                     (pointMiddle.y() - halfSize.y())
                     ));

    return rect;
}

void property_control::Show(const QRect &rect)
{
    this->show();
    this->setGeometry(rect);
}

void property_control::Hide()
{
    this->hide();
}
