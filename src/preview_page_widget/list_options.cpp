#include "list_options.h"
#include <QEvent>
#include <QLayout>
#include <QDebug>
#include "utils/common_script.h"
#include "ui/uicore.h"
#include "core/core.h"
#include "mainwindow.h"

list_options::list_options(QWidget *parent)
    : QWidget{parent}
{
    this->_del = new QPushButton(this);
    this->_copy = new QPushButton(this);
    this->_paste = new QPushButton(this);

    this->setLayout(new QVBoxLayout(this));

    QObject::connect(_copy, &QPushButton::released, this, &list_options::copy);
    QObject::connect(_paste, &QPushButton::released, this, &list_options::paste);
    QObject::connect(_del, &QPushButton::released, this, &list_options::del);

    _del->setText(tr("Delete page"));
    _copy->setText(tr("Copy page"));
    _paste->setText(tr("Paste page"));

    this->layout()->addWidget(_del);
    this->layout()->addWidget(_copy);
    this->layout()->addWidget(_paste);

    this->hide();

    UiCore::makePop(this);
    UiCore::makeZeroBorder(this);
}

list_options::~list_options()
{

}

void list_options::del()
{
    auto *main = core::get_main_window();

    main->removePage(_index);

    this->hide();
}

void list_options::paste()
{

    emit this->ClickPaste(this->_index);
    this->hide();
}

void list_options::copy()
{
    emit this->ClickCopy(this->_index);
    this->hide();
}

void list_options::Show(const QPoint &point, int index)
{
    const auto size = this->size();
    const auto realPos = point - QPoint(size.width(), size.height());

    _index = index;

    this->show();
    this->move(realPos);
}
