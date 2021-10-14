#include "remove_key.h"
#include <QSettings>
#include "../setting_define.h"

void remove_key(const char *key, const char *group)
{
    QSettings setting(ORGANIZATIONAME, APPLICATION_NAME);
    setting.beginGroup(group);

    setting.remove(key);
    setting.endGroup();
}
