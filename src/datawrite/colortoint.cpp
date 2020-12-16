#include "colortoint.h"
#include "stdlib.h"

void colortoint(QColor *color_, int *pointer){
    int **point = (int **)malloc(sizeof(int)*3);
    color_->getRgb(*point, *point + 1, *point + 2);
}
