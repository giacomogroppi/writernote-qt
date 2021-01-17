#include "../tabletcanvas.h"

void TabletCanvas::resizeEvent(QResizeEvent *)
{
    if(data == NULL)
        return;

    int i, len;
    len = data->x.length();
    if(len){
        for(i=0; i<len; i++)
            if(data->idtratto.at(i) == -1)
                break;

        short int delta;
        delta = data->x.at(i+1);
        if(delta < width() && data->x.at(i) < 0){
            delta = this->data->x.at(i);
            for(i=0; i<len; i++)
                data->x[i] += delta;
        }
    }

    initPixmap();
    this->isloading = true;
}
