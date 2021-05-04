#include "../../utils/setting_define.h"
#include "pen_ui.h"

#include <QSettings>
#include <QDebug>

void pen_ui::save_settings(){
    qDebug() << "save m_spessore_pen " << m_spessore_pen;

    QSettings setting(ORGANIZATIONAME, APPLICATION_NAME);
    setting.beginGroup(GROUP_PEN);

    setting.setValue(KEY_PEN_SIZE, m_spessore_pen);
    setting.setValue(KEY_PEN_TYPE, m_type_pen);
    setting.setValue(KEY_PEN_TIPO_TRATTO, m_type_tratto);

    setting.endGroup();

}
