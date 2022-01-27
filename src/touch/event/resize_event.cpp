#include "touch/tabletcanvas.h"

void TabletCanvas::resizeEvent(QResizeEvent *)
{
    int maxWidth;
    int maxHeigth;

    maxWidth = width();
    maxHeigth = height();

    if(unlikely(data == NULL))
        return;

    data->datatouch->controllForRepositioning();

    if(likely(!data->datatouch->isempty())){
        data->datatouch->adjustAll(maxWidth, maxHeigth);
    }

    initPixmap(true);
}
