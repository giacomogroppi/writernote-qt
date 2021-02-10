#include "last_open.h"
#include "ui_last_open.h"

#include <QSettings>
#include "../utils/setting_define.h"
#include <QAction>

#include "load_data.h"

last_open::last_open(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::last_open)
{
    ui->setupUi(this);
}

last_open::~last_open()
{
    int i, len;

    len = m_lista.length();
    for(i=0; i<len; i++){
        delete m_lista.at(i);
    }

    delete ui;
}

void last_open::setting_data(last_file *data)
{
    m_last_file = data;
}

int last_open::load_data_()
{
    QSettings setting(ORGANIZATIONAME, APPLICATION_NAME);
    setting.beginGroup(GROUPNAME_LAST_FILE);

    int quanti = setting.value(KEY_LAST_FILE_QUANTI, 0).toInt();
    int i;

    element_ui *temp_element_ui;

    QList<last_file *> temp_struct = load_data(setting, quanti);

    for(i=0; i<quanti; i++){
        temp_element_ui = new element_ui;

        temp_element_ui->setData(temp_struct.at(i));

        m_lista.append(temp_element_ui);

        ui->verticalLayout->addWidget(temp_element_ui);
    }

    setting.endGroup();

    return quanti;
}
