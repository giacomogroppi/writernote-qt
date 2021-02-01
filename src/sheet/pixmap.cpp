#include "pixmap.h"

pixmap::pixmap(QWidget *)
{
    setAutoFillBackground(true);
    setAttribute(Qt::WA_TabletTracking);
}

void pixmap::set(int *pointer){
    this->color = QColor::fromRgb(pointer[0], pointer[1], pointer[2], pointer[3]);
}

void pixmap::paintEvent(QPaintEvent *event) {
    pixmap_.fill(color);
}
