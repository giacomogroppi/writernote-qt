#ifndef STYLE_STRUCT_H
#define STYLE_STRUCT_H

#define QUANTESTRUCT 5

#define STRNOME 40

#include <QMetaType>
#include <QDataStream>

#include "../touch/datastruct/datastruct.h"


struct style_struct_S{
    char nome[STRNOME + 1];
    int nx, ny;

    struct colore_s colore;
    //int colore[4];


    int thickness;
};

struct style_struct{
    int quanti;
    struct style_struct_S style[QUANTESTRUCT];
};


Q_DECLARE_METATYPE(style_struct_S);

Q_DECLARE_METATYPE(style_struct);

#endif // STYLE_STRUCT_H
