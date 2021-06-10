#include "ui_highlighter.h"
#include "ui_ui_highlighter.h"

#include <QSettings>
#include "../../utils/setting_define.h"
#include "../pen/pen_ui.h"

ui_highlighter::ui_highlighter(QWidget *parent, bool *same, pen_ui *pen) :
    QDialog(parent),
    ui(new Ui::ui_highlighter)
{
    ui->setupUi(this);

    same_data = same;
    m_pen = pen;

    loadSettings();

    ui->slider->setMinimum(0);
    ui->slider->setMaximum(127);

    this->setWindowFlags(Qt::Window | Qt::FramelessWindowHint);

}

ui_highlighter::~ui_highlighter()
{
    delete ui;
}

void ui_highlighter::loadSettings()
{
    QSettings setting(ORGANIZATIONAME, APPLICATION_NAME);
    setting.beginGroup(GROUPNAME_HIGHLIGHTER);

    m_data.alfa = setting.value(KEY_HIGHLIGHTER_ALFA, 50).toInt();

    setting.endGroup();
    updateList();
}

void ui_highlighter::saveSettings()
{
    QSettings setting(ORGANIZATIONAME, APPLICATION_NAME);
    setting.beginGroup(GROUPNAME_HIGHLIGHTER);

    setting.setValue(KEY_HIGHLIGHTER_ALFA, m_data.alfa);

    setting.endGroup();
}

bool ui_highlighter::event(QEvent *event)
{
    if(event->type() == QEvent::WindowDeactivate)
        this->hide();

    return QWidget::event(event);

}

void ui_highlighter::updateList()
{
    ui->same_data->setChecked(this->same_data);
    if(same_data){
        ui->slider->setValue(m_pen->get_size_private()*100);
    }else{
        ui->slider->setValue(m_data.alfa);
    }
}


void ui_highlighter::on_slider_actionTriggered(int action)
{
    m_data.alfa = action;
}

void ui_highlighter::on_checkBox_stateChanged(int arg1)
{
    if(same_data){
        *same_data = arg1;
    }
}

double ui_highlighter::getSize(const double pressure){
    if(same_data){
        return m_pen->getSize(pressure);
    }

}

uchar ui_highlighter::getAlfa(){
    //return () ? m_data.alfa : m_pen;
}

