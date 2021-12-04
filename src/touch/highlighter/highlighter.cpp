#include "highlighter.h"
#include "ui_highlighter.h"
#include <QSettings>
#include "../../utils/setting_define.h"
#include "../pen/pen_ui.h"
#include "../datastruct/datastruct.h"
#include <QDebug>

#define MAX_ALFA 40
#define MIN_ALFA 1

highlighter::highlighter(QWidget *parent, bool *same, pen_ui *pen) :
    QDialog(parent),
    ui(new Ui::highlighter)
{
    ui->setupUi(this);

    same_data = same;
    m_pen = pen;

    ui->button_pressure->setCheckable(true);
    ui->button_size->setCheckable(true);

    ui->slider_alfa->setMinimum(MIN_ALFA);
    ui->slider_alfa->setMaximum(MAX_ALFA);

    this->setWindowFlags(Qt::Window | Qt::FramelessWindowHint);

    loadSettings();

    ui->slider_size->setMinimum(50);
    ui->slider_size->setMaximum(100);
}

highlighter::~highlighter()
{
    saveSettings();

    delete ui;
}

void highlighter::loadSettings()
{
    QSettings setting(ORGANIZATIONAME, APPLICATION_NAME);
    bool ok;
    setting.beginGroup(GROUPNAME_HIGHLIGHTER);

    m_data.alfa = setting.value(KEY_HIGHLIGHTER_ALFA, MAX_ALFA).toInt();

    m_data.size = setting.value(KEY_HIGHLIGHTER_SIZE, 0.05).toReal(&ok);
    if(!ok) m_data.size = 0.05;

    m_data.pressure = setting.value(KEY_HIGHLIGHTER_SPESS, true).toBool();
    m_data.tratto_sotto = setting.value(KEY_HIGHLIGHTER_SOTTO, true).toBool();

    setting.endGroup();
    updateList();
}

void highlighter::saveSettings()
{
    QSettings setting(ORGANIZATIONAME, APPLICATION_NAME);
    setting.beginGroup(GROUPNAME_HIGHLIGHTER);

    setting.setValue(KEY_HIGHLIGHTER_ALFA, m_data.alfa);
    setting.setValue(KEY_HIGHLIGHTER_SIZE, m_data.size);
    setting.setValue(KEY_HIGHLIGHTER_SPESS, m_data.pressure);
    setting.setValue(KEY_HIGHLIGHTER_SOTTO, m_data.tratto_sotto);

    setting.endGroup();
}

bool highlighter::event(QEvent *event)
{
    if(event->type() == QEvent::WindowDeactivate)
        this->hide();

    return QWidget::event(event);
}

void highlighter::updateList()
{
    bool pressure;

    if(!same_data)
        return;

    ui->same_data->setChecked(*same_data);

    ui->slider_alfa->setValue(m_data.alfa);

    if(*same_data){
        pressure = m_pen->IsPressure();
    }else{
        pressure = m_data.pressure;
        ui->slider_size->setValue(m_data.size);
    }

    ui->button_pressure->setChecked(pressure);
    ui->button_size->setChecked(!pressure);
    ui->slider_size->setEnabled(!pressure);
}

double highlighter::getSize(const double pressure){
    const double size = (m_data.pressure) ? pressure*20 : m_data.size;
    if(*same_data){
        return m_pen->getSize(pressure)*40;
    }

    return size;
}

void highlighter::on_button_size_clicked()
{
    if(*same_data){
        m_pen->setType(false);
    }else{
        m_data.pressure = 0;
    }

    m_pen->list_update();
    updateList();
}

void highlighter::on_button_pressure_clicked()
{
    if(*same_data){
        m_pen->setType(true);
    }else{
        m_data.pressure = 1;
    }

    m_pen->list_update();
    updateList();
}

void highlighter::on_same_data_stateChanged(int arg1)
{
    *same_data = arg1;

    updateList();
    m_pen->list_update();
}

void highlighter::on_checkbox_up_stateChanged(int arg1)
{
    m_data.tratto_sotto = arg1;

    updateList();
}

void highlighter::on_slider_alfa_valueChanged(int value)
{
    m_data.alfa = value;
}

void highlighter::on_slider_size_valueChanged(int value)
{
    if(*same_data){
        m_pen->setWidthTratto(double(value)/100);
    }else{
        m_data.size = double(value)/14.0;
    }
}
