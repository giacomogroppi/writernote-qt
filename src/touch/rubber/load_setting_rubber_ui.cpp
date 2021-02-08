#include "../../utils/setting_define.h"
#include "rubber_ui.h"

#include <QSettings>

void rubber_ui::load_settings(){
    QSettings setting(ORGANIZATIONAME, APPLICATION_NAME);
    setting.beginGroup(GROUPNAME_RUBBER);

    this->m_size_gomma = setting.value(KEY_RUBBER_SIZE, DEFAULT_GOMMA_SIZE).toInt();
    this->m_type_gomma = setting.value(KEY_RUBBER_TYPE, TOTALE).toInt();

    setting.endGroup();

    this->update_data();
}
