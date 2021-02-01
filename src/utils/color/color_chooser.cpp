#include "color_chooser.h"
#include <QColorDialog>

/* color_chooser returns the color even if it is not valid */
QColor color_chooser(){
    QColor color = QColorDialog::getColor(Qt::white, nullptr, "Choose color");

    return color;
}
