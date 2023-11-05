#include "style_struct.h"
#include "utils/setting_define.h"
#include "core/WOptionSetting/WOptionSettings.h"

#define DEFAULTN 40
#define DEFAULTTHICKNESS 1

// TODO: use style_struct_S load and write methods
void style_struct::loadFromByte(const WByteArray &arr)
{
    style_struct_S tmp {};
    int i, len;

    len = arr.size() / sizeof(tmp);

    for( i=0; i < len; ++i ){

        memcpy(&tmp, arr.mid(
                i*sizeof(tmp),
                (i+1)*sizeof(tmp)).constData(),
               sizeof(tmp)
        );

        this->style.append(tmp);
    }
}

void style_struct::setDefault(style_struct_S &ref)
{
    ref.colore = color_black;
    ref.nx = ref.ny = DEFAULTN;
    ref.thickness = DEFAULTTHICKNESS;
}

void style_struct::saveInArray(WByteArray &arr)
{
    int i, len;
    len = this->style.size();
    for(i=0; i<len; ++i){
        const char * ref = (const char *) at(i);
        arr.append(ref, sizeof(style_struct_S));
    }
}

style_struct::style_struct()
{
    WByteArray arr;
    WOptionSettings settings;
    settings.begin();

    default_val = settings.value(KEYDEFAULTSTYLE, 0).toInt();
    arr = settings.value(KEYSTYLE, WByteArray("")).toByteArray();

    this->loadFromByte(arr);

    settings.save();
}

void style_struct::save()
{
    WByteArray arr;
    WOptionSettings settings;
    settings.begin();
    saveInArray(arr);

    settings.setValue({KEYSTYLE}, arr);
    settings.setValue({KEYDEFAULTSTYLE}, this->default_val);

    settings.save();
}

void style_struct::saveDefault(const int index)
{
    WOptionSettings settings;
    settings.begin();
    settings.setValue({KEYDEFAULTSTYLE}, index);
    settings.save();
}

void style_struct::createNew(const WString &name)
{
    struct style_struct_S tmp;
    strncpy(tmp.nome, name.toUtf8().constData(), STRNOME);
    this->setDefault(tmp);
    this->style.append(tmp);
}
