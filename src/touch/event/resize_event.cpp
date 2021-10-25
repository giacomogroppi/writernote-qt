#include "../tabletcanvas.h"

void TabletCanvas::resizeEvent(QResizeEvent *)
{
    static uint maxWidth;
    static uint maxHeigth;

    maxWidth = width();
    maxHeigth = height();

    if(data == NULL)
        return;

    data->datatouch->controllForRepositioning();

    if(!data->datatouch->isempty()){
        data->datatouch->adjustHeight(maxHeigth);
        data->datatouch->adjustWidth(maxWidth);
    }

    initPixmap(true);
}
