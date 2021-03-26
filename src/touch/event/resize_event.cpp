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

        data->datatouch->scala_x(delta);
    }

    init_:

    initPixmap(true);
    this->isloading = true;
}
