#pragma once

#define QUANTESTRUCT 5
#define STRNOME 40
#include "touch/dataTouch/datastruct/DataStruct.h"

enum n_style: int{
    line, /* draw only line */
    white, /*
            * in this case sheet drawing draws a single
            * line, at the end of the sheet, of the
            * same color as the sheet */
    empty, /* draw from default style in last style */
    square /* draw Square */
};

struct style_struct_S{
    char nome[STRNOME + 1];
    int nx, ny;
    /*
     * nx : number of orizzonal line
     * ny : number of vertical line
    */

    class WColor colore;

    int thickness;
};

class style_struct{
private:
    WListFast<style_struct_S> style;
    int default_val = 0;

    void loadFromByte(const WByteArray &arr);
    void setDefault(style_struct_S &ref);
    void saveInArray(WByteArray & arr);
public:
    inline auto length() const {return style.size();}
    style_struct();

    void save();

    style_struct_S load_default(){
        return style.at(default_val);
    }

    const style_struct_S *at(const int i) const{ return &style.at(i);}
    style_struct_S *at_mod(const int i){return &style.operator[](i);}
    void saveDefault(int index);
    void createNew(const WString &name);
};

inline bool operator==(const style_struct& lhs, const style_struct& rhs)
{
    int i, len;
    len = lhs.length();

    unsigned char check = len == rhs.length();

    if(!check) return false;

    for(i=0; i<len; i++){
        if(memcmp(&lhs.at(i)->colore, &rhs.at(i)->colore, sizeof(WColor)) != 0)
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
