#include "touch/tabletcanvas.h"

void TabletCanvas::resizeEvent(QResizeEvent *)
{
    cint maxWidth   = width();
    cint maxHeigth  = height();

    data->datatouch->controllForRepositioning();

    if(likely(!data->datatouch->isempty())){
        data->datatouch->adjustAll(maxWidth, maxHeigth);
    }

    initPixmap(true);
}
