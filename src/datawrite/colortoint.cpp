#include "colortoint.h"

void colortoint(QColor *color_, int *pointer){
    color_->getRgb(&pointer[0], &pointer[1], &pointer[2]);
}
