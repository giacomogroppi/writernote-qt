#pragma once

#define QUANTESTRUCT 5
#define STRNOME 40
#include "touch/dataTouch/datastruct/datastruct.h"


struct style_struct_S{
    char nome[STRNOME + 1];
    int nx, ny;
    /*
     * nx : number of orizzonal line
     * ny : number of vertical line
    */

    struct colore_s colore;

    int thickness;
};

class style_struct{
private:
    QList<style_struct_S> style;
    int default_val = 0;

    void loadFromByte(const QByteArray &arr);
    void setDefault(style_struct_S &ref);
    void saveInArray(QByteArray & arr);
public:
    inline uint length() const {return style.length();}
    style_struct();

    void save();

    style_struct_S load_default(){
        return style.at(default_val);
    }

    const style_struct_S *at(const int i) const{ return &style.at(i);}
    style_struct_S *at_mod(const int i){return &style.operator[](i);}
    void saveDefault(const int index);
    void createNew(const QString &name);
};

inline bool operator==(const style_struct& lhs, const style_struct& rhs)
{
    uint i, len;
    len = lhs.length();

    uchar check = len == rhs.length();

    if(!check) return false;

    for(i=0; i<len; i++){
        if(memcmp(&lhs.at(i)->colore, &rhs.at(i)->colore, sizeof(colore_s)) != 0)
            return false;

        if(lhs.at(i)->nx != rhs.at(i)->nx)
            return false;

        if(lhs.at(i)->ny != rhs.at(i)->ny)
            return false;

        if(lhs.at(i)->thickness != rhs.at(i)->thickness)
            return false;
    }

    return true;
}
