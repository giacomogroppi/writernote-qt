//
// Created by Giacomo Groppi on 16/05/23.
//

#include "WOptionSettings.h"
#include "utils/setting_define.h"

#ifdef USE_QT
#include <QVariant>

void WOptionSettings::begin()
{
    this->settings = new QSettings(ORGANIZATIONAME, APPLICATION_NAME);
    this->settings->beginGroup("deafult");
}

void WOptionSettings::save()
{
    this->settings->endGroup();
}

void WOptionSettings::remove(const WByteArray &key)
{
    this->settings->remove(key.constData());
}

WVariant WOptionSettings::value(const WByteArray &key, const WVariant &defaultMessage)
{
    const auto value = this->settings->value(key.constData(), defaultMessage);
    return value;
}

void WOptionSettings::setValue(const WByteArray &key, const WVariant &value)
{
    this->settings->setValue(key, value);
}
#endif // USE_QT
