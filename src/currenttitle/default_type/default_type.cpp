#include "default_type.h"
#include <QSettings>
#include "../../utils/setting_define.h"

default_type::e_type_copybook default_type::load_preference_copybook()
{
    QSettings setting(ORGANIZATIONAME, APPLICATION_NAME);
    setting.beginGroup(GROURPNAME_DEFAULT_INSERT);

    e_type_copybook res = static_cast<e_type_copybook>(setting.value(KEY_DEFAULT_INSERT, e_type_copybook::pen).toInt());

    setting.endGroup();

    return res;
}

void default_type::save_preference_copybook(default_type::e_type_copybook val)
{
    QSettings setting(ORGANIZATIONAME, APPLICATION_NAME);
    setting.beginGroup(GROURPNAME_DEFAULT_INSERT);

    setting.setValue(KEY_DEFAULT_INSERT, val);
    setting.endGroup();

}
