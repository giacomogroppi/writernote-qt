#include "style_struct.h"
#include "utils/setting_define.h"
#include <QSettings>

#define DEFAULTN 40
#define DEFAULTTHICKNESS 1

void style_struct::loadFromByte(const QByteArray &arr)
{
    style_struct_S tmp;
    int i, len;

    len = arr.length() / sizeof(tmp);

    for(i=0; i<len; ++i){
        memcpy(&tmp, arr.mid(i*sizeof(tmp), (i+1)*sizeof(tmp)), sizeof(tmp));
        this->style.append(tmp);
    }
}

void style_struct::setDefault(style_struct_S &ref)
{
    ref.colore.fromColor(Qt::black);
    ref.nx = ref.ny = DEFAULTN;
    ref.thickness = DEFAULTTHICKNESS;
}

void style_struct::saveInArray(QByteArray &arr)
{
    int i, len;
    len = this->style.length();
    for(i=0; i<len; ++i){
        const char * ref = (const char *) at(i);
        arr.append(ref, sizeof(style_struct_S));
    }
}

style_struct::style_struct()
{
    QByteArray arr;
    QSettings setting(ORGANIZATIONAME, APPLICATION_NAME);
    setting.beginGroup(GROUPNAME_STYLE);

    default_val = setting.value(KEYDEFAULTSTYLE, 0).toInt();
    arr = setting.value(KEYSTYLE, "").toByteArray();

    this->loadFromByte(arr);

    setting.endGroup();
}

void style_struct::save()
{
    QByteArray arr;
    QSettings setting(ORGANIZATIONAME, APPLICATION_NAME);
    setting.beginGroup(GROUPNAME_STYLE);
    saveInArray(arr);
    setting.setValue(KEYSTYLE, arr);
    setting.setValue(KEYDEFAULTSTYLE, this->default_val);
    setting.endGroup();
}

void style_struct::saveDefault(const int index)
{
    QSettings setting(ORGANIZATIONAME, APPLICATION_NAME);
    setting.beginGroup(GROUPNAME_STYLE);
    setting.setValue(KEYDEFAULTSTYLE, index);
    setting.endGroup();
}

void style_struct::createNew(const QString &name)
{
    struct style_struct_S tmp;
    strncpy(tmp.nome, name.toUtf8().constData(), STRNOME);
    this->setDefault(tmp);
    this->style.append(tmp);
}
