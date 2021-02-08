#include "../../utils/setting_define.h"
#include "pen_ui.h"

#include <QSettings>

void pen_ui::load_settings(){
    QSettings setting(ORGANIZATIONAME, APPLICATION_NAME);
    setting.beginGroup(GROUP_PEN);

    this->m_spessore_pen = setting.value(KEY_PEN_SIZE, DEFAULTSPESSORE).toInt();
    this->m_type_pen = setting.value(KEY_PEN_TYPE, DEFAULT_TYPE).toInt();
    this->m_type_tratto = setting.value(KEY_PEN_TIPO_TRATTO, CONTINUA).toInt();

    setting.endGroup();

    this->list_update();
}
