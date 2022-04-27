#include "list_options.h"
#include <QEvent>
#include <QLayout>
#include <QDebug>
#include "utils/common_script.h"
#include "ui/uicore.h"

list_options::list_options(QWidget *parent)
    : QWidget{parent}
{
    _del = new QPushButton(this);
    this->setLayout(new QHBoxLayout(this));

    QObject::connect(_del, &QPushButton::released, this, &list_options::del);

    _del->setText("Delete page");

    this->layout()->addWidget(_del);
    this->hide();
    UiCore::makePop(this);
}

list_options::~list_options()
{

}

void list_options::del()
{
    this->hide();
}

void list_options::Show(const QPoint &point)
{
    const auto size = this->size();
    const auto realPos = point - QPoint(size.width(), size.height());

    this->show();
    this->move(realPos);
}
