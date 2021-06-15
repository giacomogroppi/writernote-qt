#include "../../utils/setting_define.h"
#include "pen_ui.h"
#include <QDebug>
#include <QSettings>

void pen_ui::load_settings(){
    QSettings setting(ORGANIZATIONAME, APPLICATION_NAME);
    setting.beginGroup(GROUP_PEN);

    bool ok;

    m_spessore_pen = setting.value(KEY_PEN_SIZE, DefaultS).toDouble(&ok);
    m_type_pen = static_cast<n_pressione>(setting.value(KEY_PEN_TYPE, n_pressione::pressione).toInt());
    m_type_tratto = static_cast<n_tratto>(setting.value(KEY_PEN_TIPO_TRATTO, n_tratto::continua).toInt());

    if(!ok){
        m_spessore_pen = DefaultS;
    }

    same_data = setting.value(KEY_PEN_SAME_DATA, false).toBool();

    setting.endGroup();

    this->list_update();
}
