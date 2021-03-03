#include "rubber_ui.h"
#include "ui_rubber_ui.h"

rubber_ui::rubber_ui(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::rubber_ui)
{
    ui->setupUi(this);

    this->load_settings();

    this->setWindowFlags(Qt::Window | Qt::FramelessWindowHint);

    ui->totale_button->setCheckable(true);
    ui->partial_button->setCheckable(true);

}


rubber_ui::~rubber_ui()
{
    delete ui;
    this->save_settings();
}

bool rubber_ui::event(QEvent *event){
    if(event->type() == QEvent::WindowDeactivate)
        this->hide();

    return QWidget::event(event);
}

void rubber_ui::on_totale_button_clicked()
{
    this->m_type_gomma = TOTALE;
    ui->totale_button->setChecked(this->m_type_gomma == TOTALE);
    ui->partial_button->setChecked(this->m_type_gomma == PARZIALE);
}

void rubber_ui::on_partial_button_clicked()
{
    m_type_gomma = PARZIALE;
    ui->totale_button->setChecked(this->m_type_gomma == TOTALE);
    ui->partial_button->setChecked(this->m_type_gomma == PARZIALE);
}
