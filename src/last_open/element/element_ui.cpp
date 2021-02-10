#include "element_ui.h"
#include "ui_element_ui.h"

#include <QDebug>

element_ui::element_ui(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::element_ui)
{
    ui->setupUi(this);
}

element_ui::~element_ui()
{
    delete ui;
}

void element_ui::setData(last_file *data, int index)
{
    m_data = data;
    m_index = index;
    set_main();
}

void element_ui::set_main()
{
    ui->label_last_edit->setText(m_data->last_modification);
    ui->label_path->setText(m_data->posizione);

    if(m_data->type == TYPE_CLOUD){
        ui->label_where->setText("Cloud");
    }
    else{
        ui->label_where->setText("Local");
    }
}

bool element_ui::event(QEvent *event)
{
    qDebug() << event->type();

    if(event->type() == QEvent::MouseButtonPress)
        emit on_pressed(this->m_index);

    return QWidget::event(event);
}
