#include "last_open.h"
#include "ui_last_open.h"

#include <QSettings>
#include "../utils/setting_define.h"
#include <QAction>

last_open::last_open(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::last_open)
{
    ui->setupUi(this);

    load_data();
}

last_open::~last_open()
{
    delete ui;
}

void last_open::setting_data(last_file *data)
{
    m_last_file = data;
}

int last_open::load_data()
{
    QSettings setting(ORGANIZATIONAME, APPLICATION_NAME);
    setting.beginGroup(GROUPNAME_LAST_FILE);

    int quanti = setting.value(KEY_LAST_FILE_QUANTI, 0).toInt();

    int i;

    QList<QAction *> lista;
    QAction *temp;

    QString temp_testo;

    for(i=0; i<quanti; i++){
        temp = new QAction;

        temp_testo = setting.value((QString)KAY_BASE_FILE + QString::number(i)).toString();

        temp->setText(temp_testo);

        lista.append(temp);
    }

    setting.endGroup();

    return quanti;
}
