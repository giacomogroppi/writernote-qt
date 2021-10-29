#include "topdf.h"

static qreal pressureToWidth(qreal pressure)
{
    return pressure * 10 + 1;
}


/* la funzione è responsabile del settaggio dello spessore e del tipo per il load */
void topdf::updateBrush_load(const double pressure, QColor color){
    penna.setWidthF(pressureToWidth(pressure/2.00));
    penna.setColor(color);
}
