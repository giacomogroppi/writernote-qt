#include "pen_ui.h"
#include "ui_pen_ui.h"

pen_ui::pen_ui(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::pen_ui)
{
    ui->setupUi(this);

    ui->slider_size->setMinimum(1);
    ui->slider_size->setMaximum(5);

    ui->button_continua->setCheckable(true);
    ui->button_pressure->setCheckable(true);
    ui->button_size->setCheckable(true);
    ui->button_tratti->setCheckable(true);

    load_settings();

    this->setWindowFlags(Qt::Window | Qt::FramelessWindowHint);

    m_last_color.ok = false;
}

pen_ui::~pen_ui()
{
    save_settings();

    delete ui;
}

void pen_ui::list_update(){
    ui->slider_size->setValue(m_spessore_pen);

    ui->button_continua->setChecked(m_type_tratto == n_tratto::continua);
    ui->button_tratti->setChecked(m_type_tratto == n_tratto::tratti);

    ui->button_pressure->setChecked(m_type_pen == n_pressione::pressione);

    const bool temp = m_type_pen == n_pressione::spessore;
    ui->button_size->setChecked(temp);

    ui->slider_size->setDisabled(temp);
    ui->slider_size->setSliderPosition(this->m_spessore_pen);
}

void pen_ui::on_slider_size_valueChanged(int value)
{
    m_spessore_pen = value;
}

void pen_ui::on_button_continua_clicked()
{
    m_type_tratto = n_tratto::continua;

    this->list_update();
}

void pen_ui::on_button_tratti_clicked()
{
    m_type_tratto = n_tratto::tratti;

    this->list_update();
}

void pen_ui::on_button_pressure_clicked()
{
    m_type_pen = n_pressione::pressione;

   this->list_update();
}

void pen_ui::on_button_size_clicked()
{
    m_type_pen = n_pressione::spessore;

    this->list_update();
}

bool pen_ui::event(QEvent *event){
    if(event->type() == QEvent::WindowDeactivate)
        this->hide();

    return QWidget::event(event);
}
