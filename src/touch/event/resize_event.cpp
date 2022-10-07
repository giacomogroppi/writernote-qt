#include "touch/tabletcanvas.h"

void TabletCanvas::resizeEvent(QResizeEvent *)
{
    cint maxWidth   = width();
    cint maxHeigth  = height();

    getDoc()->controllForRepositioning();

    if(likely(!getDoc()->isempty_touch())){
        getDoc()->adjustAll(maxWidth, maxHeigth);
    }

    initPixmap(true);
}
