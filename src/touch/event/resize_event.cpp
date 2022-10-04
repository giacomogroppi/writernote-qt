#include "touch/tabletcanvas.h"

void TabletCanvas::resizeEvent(QResizeEvent *)
{
    cint maxWidth   = width();
    cint maxHeigth  = height();

    getDoc()->datatouch->controllForRepositioning();

    if(likely(!getDoc()->datatouch->isempty())){
        getDoc()->datatouch->adjustAll(maxWidth, maxHeigth);
    }

    initPixmap(true);
}
