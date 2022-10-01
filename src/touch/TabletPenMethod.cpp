#include "TabletPenMethod.h"
#include "utils/common_script.h"
#include "utils/setting_define.h"
#include <QSettings>

void TabletPenMethod::save() const
{
    QSettings setting(ORGANIZATIONAME, APPLICATION_NAME);
    setting.beginGroup(GROUPNAME_METHOD_TOUCH);

    setting.setValue(KEY_METHOD_TOUCH, _method);
    setting.endGroup();
}

void TabletPenMethod::load()
{
    QSettings setting(ORGANIZATIONAME, APPLICATION_NAME);
    setting.beginGroup(GROUPNAME_METHOD_TOUCH);

    _method = (setting.value(KEY_METHOD_TOUCH, PrivateTabletMethod_Pen) .toInt());

    setting.endGroup();
}

TabletPenMethod::TabletPenMethod(const TabletPenMethod &t)
{
    this->_method = t._method;
}

TabletPenMethod::TabletPenMethod(bool FirstLoad)
{
    W_ASSERT(FirstLoad == true);
    this->load();
}

TabletPenMethod::TabletPenMethod()
{
    this->_method = PrivateTabletMethod_Pen;
}
