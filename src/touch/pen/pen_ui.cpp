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
}

pen_ui::~pen_ui()
{
    save_settings();

    delete ui;
}

void pen_ui::list_update(){
    ui->slider_size->setValue(m_spessore_pen);

    ui->button_continua->setChecked(m_type_tratto == CONTINUA);
    ui->button_tratti->setChecked(m_type_tratto == TRATTI);

    ui->button_pressure->setChecked(m_type_pen == PRESSIONE);
    ui->button_size->setChecked(m_type_pen == SPESSORE);
}

void pen_ui::on_slider_size_valueChanged(int value)
{
    m_spessore_pen = value;
}

void pen_ui::on_button_continua_clicked()
{
    m_type_tratto = CONTINUA;

    ui->button_continua->setChecked(m_type_tratto == CONTINUA);
    ui->button_tratti->setChecked(m_type_tratto == TRATTI);
}

void pen_ui::on_button_tratti_clicked()
{
    m_type_tratto = TRATTI;

    ui->button_continua->setChecked(m_type_tratto == CONTINUA);
    ui->button_tratti->setChecked(m_type_tratto == TRATTI);
}

void pen_ui::on_button_pressure_clicked()
{
    m_type_pen = PRESSIONE;

    ui->button_pressure->setChecked(m_type_pen == PRESSIONE);
    ui->button_size->setChecked(m_type_pen == SPESSORE);
}

void pen_ui::on_button_size_clicked()
{
    m_type_pen = SPESSORE;

    ui->button_pressure->setChecked(m_type_pen == PRESSIONE);
    ui->button_size->setChecked(m_type_pen == SPESSORE);
}
