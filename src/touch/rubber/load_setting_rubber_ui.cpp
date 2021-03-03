#include "../../utils/setting_define.h"
#include "rubber_ui.h"
#include "ui_rubber_ui.h"

#include <QSettings>

void rubber_ui::load_settings(){
    QSettings setting(ORGANIZATIONAME, APPLICATION_NAME);
    setting.beginGroup(GROUPNAME_RUBBER);

    this->m_size_gomma = setting.value(KEY_RUBBER_SIZE, DEFAULT_GOMMA_SIZE).toInt();
    this->m_type_gomma = setting.value(KEY_RUBBER_TYPE, TOTALE).toInt();

    ui->totale_button->setCheckable(true);
    ui->partial_button->setCheckable(true);

    ui->totale_button->setChecked(m_type_gomma == TOTALE);
    ui->partial_button->setChecked(m_type_gomma == PARZIALE);

    setting.endGroup();

    this->update_data();
}
