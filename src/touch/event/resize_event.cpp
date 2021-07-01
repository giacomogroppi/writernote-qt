#include "../tabletcanvas.h"

void TabletCanvas::resizeEvent(QResizeEvent *)
{
    if(data == NULL)
        return;

    data->datatouch->controllForRepositioning();


    initPixmap(true);
    this->isloading = true;

    if(!data->datatouch->isempty())
        data->datatouch->adjustHeight(m_pixmap.height(), false);

}
