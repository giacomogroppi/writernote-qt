#include "load_last_style.h"

#include <QSettings>
#include <QtCore/QDataStream>
#include <QtCore/QSettings>

#define DEFAULTTHICKNESS 1
#define DEFAULTN 20
#define DEFAULTNOME "Undefined"

#define DEFAULTALFA 254

#include "../utils/setting_define.h"

#include "string.h"

void load_default(style_struct *default_setting){
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

void load_default_onlystyle( style_struct_S *default_setting){
    strcpy(default_setting->nome, DEFAULTNOME);

    default_setting->colore[1] = default_setting->colore[0] = 0;
    default_setting->colore[2] = 255;
    default_setting->colore[3] = DEFAULTALFA;

    default_setting->nx = default_setting->ny = DEFAULTN;

    default_setting->thickness = DEFAULTTHICKNESS;

}

int load_default_drawing_index(){
    QSettings setting(ORGANIZATIONAME, APPLICATIONAME);
    setting.beginGroup(GROUPNAME_STYLE);

    bool ok;

    int indice = setting.value(KEYDEFAULTSTYLE, QVariant::fromValue(0)).toInt(&ok);

    setting.endGroup();

    if(!ok)
        return -1;
    return indice;

}

style_struct_S * load_default_drawing(){
    style_struct *style_temp;

    int indice = load_default_drawing_index();

    if(indice == -1)
        return NULL;

    style_temp = load_last_style();

    return &style_temp->style[indice];
}

void save_default_drawing(int *data){
    QSettings setting(ORGANIZATIONAME, APPLICATIONAME);
    setting.beginGroup(GROUPNAME_STYLE);

    setting.setValue(KEYDEFAULTSTYLE, *data);

    setting.endGroup();
}

style_struct * load_last_style(){
    style_struct default_setting;

    load_default(&default_setting);

    style_struct *style_temp = new style_struct;

    QSettings setting(ORGANIZATIONAME, APPLICATIONAME);
    setting.beginGroup(GROUPNAME_STYLE);

    *style_temp = setting.value(KEYSTYLE, QVariant::fromValue(default_setting)).value<style_struct>();

    setting.endGroup();

    return style_temp;
}

void save_last_style(style_struct *style_v){
    QSettings setting(ORGANIZATIONAME, APPLICATIONAME);
    setting.beginGroup(GROUPNAME_STYLE);

    setting.setValue(KEYSTYLE, QVariant::fromValue(*style_v));

    setting.endGroup();
}
