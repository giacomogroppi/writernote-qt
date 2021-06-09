#include "ui_highlighter.h"
#include "ui_ui_highlighter.h"

#include <QSettings>
#include "../../utils/setting_define.h"

ui_highlighter::ui_highlighter(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ui_highlighter)
{
    ui->setupUi(this);

    loadSettings();

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
}

void ui_highlighter::saveSettings()
{
    QSettings setting(ORGANIZATIONAME, APPLICATION_NAME);
    setting.beginGroup(GROUPNAME_HIGHLIGHTER);

    setting.setValue(KEY_HIGHLIGHTER_ALFA, m_data.alfa);

    setting.endGroup();
}
