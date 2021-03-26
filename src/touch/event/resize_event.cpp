#include "../tabletcanvas.h"

void TabletCanvas::resizeEvent(QResizeEvent *)
{
    if(data == NULL)
        return;

    if(!data->datatouch->isempty()){
        double delta;

        if(!data->datatouch->maxXIdOrizzonal(&delta)){
            goto init_;
        }

        if(delta < width() && data->datatouch->m_point.)

        data->datatouch->scala_x(delta);
    }

    init_:

    initPixmap(true);
    this->isloading = true;
}
