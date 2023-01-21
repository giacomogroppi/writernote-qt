#include "highlighter.h"
#include "ui_highlighter.h"
#include <QSettings>
#include "utils/setting_define.h"
#include "touch/pen/pen_ui.h"
#include "touch/dataTouch/datastruct/datastruct.h"
#include "touch/dataTouch/stroke/StrokePre.h"
#include <QDebug>
#include <utility>

#define MAX_ALFA 150
#define MIN_ALFA 20

extern StrokePre __tmp;

highlighter::highlighter(QWidget *parent,
                         bool &same,
                         pen_ui *m_pen,
                         std::function<int()> getTime,
                         QPen &pen,
                         QColor &color) :
    QDialog(parent),
    HighligterMethod(std::move(getTime),
                     [&](double size) -> pressure_t { return this->getSize(size); },
                     pen,
                     color
                     ),
    _same_data(same),
    _pen(m_pen),
    ui(new Ui::highlighter)
{
    ui->setupUi(this);

    ui->button_pressure->setCheckable(true);
    ui->button_size->setCheckable(true);

    ui->slider_alfa->setMinimum(MIN_ALFA);
    ui->slider_alfa->setMaximum(MAX_ALFA);

    this->setWindowFlags(Qt::Popup | Qt::FramelessWindowHint);

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

    if(!ok)
        m_data.size = 50;

    m_data.pressure = setting.value(KEY_HIGHLIGHTER_SPESS, true).toBool();

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

    ui->same_data->setChecked(_same_data);

    ui->slider_alfa->setValue(m_data.alfa);

    if(_same_data){
        pressure = _pen->IsPressure();
    }else{
        pressure = m_data.pressure;
        ui->slider_size->setValue(m_data.size);
    }

    ui->button_pressure->setChecked(pressure);
    ui->button_size->setChecked(!pressure);
    ui->slider_size->setEnabled(!pressure);
}

double highlighter::getSize(const double pressure)
{
    const double size = (m_data.pressure) ? pressure*20 : m_data.size;
    if(_same_data){
        return _pen->getSize(pressure)*40;
    }

    return size;
}

void highlighter::on_button_size_clicked()
{
    if(_same_data){
        _pen->setType(false);
    }else{
        m_data.pressure = 0;
    }

    _pen->list_update();
    updateList();
}

void highlighter::on_button_pressure_clicked()
{
    if(_same_data){
        _pen->setType(true);
    }else{
        m_data.pressure = 1;
    }

    _pen->list_update();
    updateList();
}

void highlighter::on_same_data_stateChanged(int arg1)
{
    _same_data = arg1;

    updateList();
    _pen->list_update();
}

void highlighter::on_slider_alfa_valueChanged(int value)
{
    m_data.alfa = value;
}

void highlighter::on_slider_size_valueChanged(int value)
{
    if(_same_data){
        _pen->setWidthTratto(double(value)/100);
    }else{
        m_data.size = double(value)/14.0;
    }
}
