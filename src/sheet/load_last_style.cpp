#include "load_last_style.h"

#include <QSettings>
#include <QtCore/QDataStream>
#include <QtCore/QSettings>

#define DEFAULTTHICKNESS 5
#define DEFAULTN 20
#define DEFAULTNOME "Undefined"

#define DEFAULTALFA 254

#include "string.h"

void load_default(struct style_struct *default_setting){
    int i;
    default_setting->quanti = 0;
    for(i=0; i<QUANTESTRUCT; i++){
        strcpy(default_setting->style[i].nome, DEFAULTNOME);

        default_setting->style[i].colore[1] = default_setting->style[i].colore[0] = 0;
        default_setting->style[i].colore[2] = 255;
        default_setting->style[i].colore[3] = DEFAULTALFA;

        default_setting->style[i].nx = default_setting->style[i].ny = DEFAULTN;

        default_setting->style[i].thickness = DEFAULTTHICKNESS;
    }
}

style_struct_S * load_default_drawing(){
    style_struct default_setting;

    load_default(&default_setting);

    style_struct_S *style_temp = new style_struct_S;

    QSettings setting("writernote", "style");
    setting.beginGroup("style");

    QVariant value = setting.value("style_form_default", QVariant::fromValue(default_setting));

    *style_temp = value.value<style_struct_S>();

    setting.endGroup();

    return style_temp;
}

void save_default_drawing(struct style_struct_S *data){
    QSettings setting("writernote", "style");
    setting.beginGroup("style");

    setting.setValue("style_form_default", QVariant::fromValue(*data));

    setting.endGroup();
}

style_struct * load_last_style(){
    style_struct default_setting;

    load_default(&default_setting);

    style_struct *style_temp = new style_struct;

    QSettings setting("writernote", "style");
    setting.beginGroup("style");

    QVariant value = setting.value("style_form", QVariant::fromValue(default_setting));

    *style_temp = value.value<style_struct>();

    setting.endGroup();

    return style_temp;
}

int save_last_style(style_struct *style_v){
    QSettings setting("writernote", "style");
    setting.beginGroup("style");

    setting.setValue("style_form", QVariant::fromValue(*style_v));

    setting.endGroup();

    return 1;
}


