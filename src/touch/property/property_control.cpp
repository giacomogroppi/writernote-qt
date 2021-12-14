#include "property_control.h"
#include "ui_property_control.h"

#define SET_PRIVATE_STYLE(button) button->setStyleSheet("background-color: rgba(255, 255, 255, 255)");

property_control::property_control(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::property_control)
{
    ui->setupUi(this);

    setStyleSheet("background:transparent;");
    this->setWindowFlags(Qt::Window | Qt::FramelessWindowHint);
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
    ui->button_copy->setEnabled(     (flags & PROPERTY_SHOW_COPY));
    ui->button_cut->setEnabled(      (flags & PROPERTY_SHOW_CUT));
    ui->button_delete->setEnabled(   (flags & PROPERTY_SHOW_DELETE));
    ui->button_paste->setEnabled(    (flags & PROPERTY_SHOW_PASTE));

    if(flags){
        this->show();
        this->raise();
    }
    else{
        this->hide();
    }    

    this->move(point);

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

