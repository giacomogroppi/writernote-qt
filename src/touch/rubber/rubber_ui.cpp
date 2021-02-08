#include "rubber_ui.h"
#include "ui_rubber_ui.h"

#include <QDebug>

rubber_ui::rubber_ui(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::rubber_ui)
{
    ui->setupUi(this);

    this->setWindowFlags(Qt::Window | Qt::FramelessWindowHint);
}

rubber_ui::~rubber_ui()
{
    delete ui;
}

bool rubber_ui::event(QEvent *event){
    qDebug() << event->type();

    return QWidget::event(event);
}
