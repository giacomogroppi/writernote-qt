#include "../tabletcanvas.h"

void TabletCanvas::resizeEvent(QResizeEvent *)
{
    if(data == NULL)
        return;

    int i, len;
    len = data->datatouch->x.length();
    if(len){
        for(i=0; i<len; i++)
            if(data->datatouch->idtratto.at(i) == -1)
                break;

        short int delta;
        delta = data->datatouch->x.at(i+1);
        if(delta < width() && data->datatouch->x.at(i) < 0){
            delta = data->datatouch->x.at(i);
            for(i=0; i<len; i++)
                data->datatouch->x[i] += delta;
        }
    }

    initPixmap();
    this->isloading = true;
}
