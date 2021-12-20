#include "touch/tabletcanvas.h"
#include <QSettings>
#include "utils/setting_define.h"

void TabletCanvas::saveScrollingSetting(){
    QSettings setting(ORGANIZATIONAME, APPLICATION_NAME);
    setting.beginGroup(GROUPNAME_SPEED_SCROLLING);


    setting.setValue(KEY_SPEED_SCROLLING, this->m_scrolling_speed);
    setting.setValue(KEY_SPEED_SCROLLING_ENABLE, this->m_scrolling_speed_enable);

    setting.endGroup();
}

void TabletCanvas::loadScrollinSetting(){
    QSettings setting(ORGANIZATIONAME, APPLICATION_NAME);
    setting.beginGroup(GROUPNAME_SPEED_SCROLLING);

    bool ok;
    this->m_scrolling_speed = setting.value(KEY_SPEED_SCROLLING, default_speed_value).toInt(&ok);

    if(!ok){
        m_scrolling_speed = default_speed_value;
    }

    this->m_scrolling_speed_enable = setting.value(KEY_SPEED_SCROLLING_ENABLE, true).toBool();

    setting.endGroup();
}
