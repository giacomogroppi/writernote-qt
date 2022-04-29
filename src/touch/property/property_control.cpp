#include "property_control.h"
#include "ui_property_control.h"
#include "touch/tabletcanvas.h"
#include <QMouseEvent>
#include <QTabletEvent>

#define SET_PRIVATE_STYLE(button)   button->setStyleSheet("background-color: rgba(255, 255, 255, 255)");

property_control::property_control(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::property_control)
{
    W_ASSERT(this->parentWidget()->objectName() == "TabletCanvas");
    ui->setupUi(this);

    setStyleSheet("background:transparent;");
    this->setWindowFlags(Qt::Popup | Qt::FramelessWindowHint);
    this->setAttribute(Qt::WA_TranslucentBackground);

    SET_PRIVATE_STYLE(ui->button_copy);
    SET_PRIVATE_STYLE(ui->button_cut);
    SET_PRIVATE_STYLE(ui->button_delete);
    SET_PRIVATE_STYLE(ui->button_paste);
}

property_control::~property_control()
{
    delete ui;
}

void property_control::Show(const QPoint &point, int flags)
{
    constexpr auto not_used prop_debug = true;
    constexpr auto not_used name_func = "property_control::Show(const QPointF &point, int flags)";

    ui->button_copy->setEnabled(     (flags & PROPERTY_SHOW_COPY));
    ui->button_cut->setEnabled(      (flags & PROPERTY_SHOW_CUT));
    ui->button_delete->setEnabled(   (flags & PROPERTY_SHOW_DELETE));
    ui->button_paste->setEnabled(    (flags & PROPERTY_SHOW_PASTE));

    if(likely(flags)){
        this->show();
        this->raise();
    }
    else{
        this->hide();
    }    

    this->move(point);

    WDebug(prop_debug, name_func << point);

}

void property_control::on_button_copy_clicked()
{
    emit ActionSelection(ActionProperty::__copy);
}

void property_control::on_button_cut_clicked()
{
    emit ActionSelection(ActionProperty::__cut);
}

void property_control::on_button_delete_clicked()
{
    emit ActionSelection(ActionProperty::__delete);
}

void property_control::on_button_paste_clicked()
{
    emit ActionSelection(ActionProperty::__paste);
}

bool property_control::event(QEvent *event)
{
    //qDebug() << "property_control" << __FUNCTION__;
    return QWidget::event(event);
}

