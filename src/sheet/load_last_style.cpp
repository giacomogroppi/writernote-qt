#include "load_last_style.h"

#include <QSettings>
#include <QtCore/QDataStream>
#include <QtCore/QSettings>



#include "../utils/setting_define.h"

#include "string.h"

static void load_default_(style_struct *data, int indice, const char *name){
    strcpy(data->style[indice].nome, name);

    data->style[indice].colore[1] = data->style[indice].colore[0] = 0;
    data->style[indice].colore[2] = 255;
    data->style[indice].colore[3] = DEFAULTALFA;

    data->style[indice].nx = data->style[indice].ny = DEFAULTN;

    data->style[indice].thickness = DEFAULTTHICKNESS;
}

void load_default(style_struct *default_setting, const char *name, int indice){
    int i;
    default_setting->quanti = 0;

    if(indice != -1){
        return load_default_(default_setting, indice, name);
    }

    for(i=0; i<QUANTESTRUCT; i++){
        load_default_(default_setting, i, DEFAULTNOME);
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

    int indice = setting.value(KEYDEFAULTSTYLE, QVariant::fromValue(-1)).toInt(&ok);

    setting.endGroup();

    if(!ok)
        return -1;

    return indice;

}

/* la funzione è utilizzata per aver subito la struttura di che cosa deve disegnare */
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

    load_default(&default_setting, DEFAULTNOME);

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
