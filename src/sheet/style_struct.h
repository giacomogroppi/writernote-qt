#ifndef STYLE_STRUCT_H
#define STYLE_STRUCT_H

#include <QMetaType>

struct style_struct{
    int nx, ny;
    int colore[4];
    int thickness;
};


Q_DECLARE_METATYPE(style_struct);


#endif // STYLE_STRUCT_H
