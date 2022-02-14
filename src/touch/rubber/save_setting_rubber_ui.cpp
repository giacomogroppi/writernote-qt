#include "utils/setting_define.h"
#include "rubber_ui.h"

#include <QSettings>

void rubber_ui::save_settings(){
    QSettings setting(ORGANIZATIONAME, APPLICATION_NAME);
    setting.beginGroup(GROUPNAME_RUBBER);

    setting.setValue(KEY_RUBBER_SIZE, _size_gomma);
    setting.setValue(KEY_RUBBER_TYPE, _type_gomma);

    setting.endGroup();

}
