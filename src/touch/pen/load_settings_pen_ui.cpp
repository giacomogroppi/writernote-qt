#include "../../utils/setting_define.h"
#include "pen_ui.h"

#include <QSettings>

void pen_ui::load_settings(){
    QSettings setting(ORGANIZATIONAME, APPLICATION_NAME);
    setting.beginGroup(GROUP_PEN);

    this->m_spessore_pen = setting.value(KEY_PEN_SIZE, DefaultS).toInt();
    this->m_type_pen = static_cast<n_pressione>(setting.value(KEY_PEN_TYPE, n_pressione::pressione).toInt());
    this->m_type_tratto = static_cast<n_tratto>(setting.value(KEY_PEN_TIPO_TRATTO, n_tratto::continua).toInt());

    setting.endGroup();

    this->list_update();
}
