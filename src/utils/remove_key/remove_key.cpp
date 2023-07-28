#include "remove_key.h"
#include "utils/setting_define.h"
#include "core/WOptionSetting/WOptionSettings.h"

void remove_key(const char *key, const char *group)
{
    WOptionSettings settings;
    settings.begin();

    settings.remove(key);
    settings.save();
}
