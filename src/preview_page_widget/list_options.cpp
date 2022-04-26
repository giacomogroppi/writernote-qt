#include "list_options.h"
#include <QLayout>

list_options::list_options(QWidget *parent)
    : QWidget{parent}
{
    _del = new QPushButton(this);

    QObject::connect(_del, &QPushButton::released, this, &list_options::del);

    this->layout()->addWidget(_del);
}

list_options::~list_options()
{

}

void list_options::del()
{
    this->hide();
}
