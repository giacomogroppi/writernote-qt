#include "utils/setting_define.h"
#include "rubber_ui.h"
#include "ui_rubber_ui.h"

#include <QSettings>

void rubber_ui::load_settings(){
    QSettings setting(ORGANIZATIONAME, APPLICATION_NAME);
    setting.beginGroup(GROUPNAME_RUBBER);

    this->m_size_gomma = setting.value(KEY_RUBBER_SIZE, DEFAULT_GOMMA_SIZE).toInt();
    this->m_type_gomma = static_cast<e_type_rubber>(
                setting.value(KEY_RUBBER_TYPE, e_type_rubber::total).toInt()
                                                    );

    ui->totale_button->setCheckable(true);
    ui->partial_button->setCheckable(true);

    setting.endGroup();

    this->update_data();
}
