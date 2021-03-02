#include "setting_color.h"

QColor settaggiocolore(int *colore_){
    QColor colore = QColor::fromRgb(colore_[0],
            colore_[1],
            colore_[2],
            colore_[3]);
    return colore;
}


