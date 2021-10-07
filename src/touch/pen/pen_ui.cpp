#include "pen_ui.h"
#include "ui_pen_ui.h"
#include <QSettings>
#include <QDebug>
#define DELTA 200.0
#include "../highlighter/highlighter.h"
#include "../../utils/setting_define.h"

pen_ui::pen_ui(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::pen_ui)
{
    ui->setupUi(this);

    ui->slider_size->setMinimum(1);
    ui->slider_size->setMaximum(200);

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
    const double currentSize = (m_spessore_pen*DELTA);
    const bool spessore = m_type_pen == n_pressione::spessore;
    ui->checkBox->setChecked(same_data);

    ui->slider_size->setValue(m_spessore_pen);

    ui->button_continua->setChecked(m_type_tratto == n_tratto::continua);
    ui->button_tratti->setChecked(m_type_tratto == n_tratto::tratti);

    ui->button_pressure->setChecked(m_type_pen == n_pressione::pressione);

    ui->button_size->setChecked(spessore);

    ui->slider_size->setSliderPosition(currentSize);
    ui->slider_size->setEnabled(spessore);

    if(same_data && m_highlighter){
        this->m_highlighter->updateList();
    }
}

void pen_ui::on_slider_size_valueChanged(int value)
{
    m_spessore_pen = double(value)/DELTA;
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

void pen_ui::on_checkBox_stateChanged(int arg1)
{
    same_data = arg1;
    if(m_highlighter)
        this->m_highlighter->updateList();
}

void pen_ui::load_settings(){
    bool ok;
    QSettings setting(ORGANIZATIONAME, APPLICATION_NAME);
    setting.beginGroup(GROUP_PEN);

    m_spessore_pen = setting.value(KEY_PEN_SIZE, DefaultS).toDouble(&ok);
    m_type_pen = static_cast<n_pressione>(setting.value(KEY_PEN_TYPE, n_pressione::pressione).toInt());
    m_type_tratto = static_cast<n_tratto>(setting.value(KEY_PEN_TIPO_TRATTO, n_tratto::continua).toInt());

    if(!ok){
        m_spessore_pen = DefaultS;
    }

    same_data = setting.value(KEY_PEN_SAME_DATA, false).toBool();

    setting.endGroup();

    this->list_update();
}

void pen_ui::save_settings(){
    QSettings setting(ORGANIZATIONAME, APPLICATION_NAME);
    setting.beginGroup(GROUP_PEN);

    setting.setValue(KEY_PEN_SIZE, m_spessore_pen);
    setting.setValue(KEY_PEN_TYPE, m_type_pen);
    setting.setValue(KEY_PEN_TIPO_TRATTO, m_type_tratto);
    setting.setValue(KEY_PEN_SAME_DATA, same_data);

    setting.endGroup();
}
