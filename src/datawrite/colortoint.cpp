#include "colortoint.h"

void colortoint(QColor *color_, int *pointer){
    color_->getRgb(pointer, pointer + 1, pointer + 2);
}
