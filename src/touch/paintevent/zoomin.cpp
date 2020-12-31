#include "../tabletcanvas.h"
#include <QDebug>

/* automenta lo zoom */
void TabletCanvas::zoomin(QPaintEvent *event){
    if(!this->iszoomin) return;

    if(data->zoom >= 2.0)
        return;

    if(m_pixmap.width() < width())
        this->initPixmap(-1);
    else{
        int i, len = data->x.length();

        for(i=0; i < len; i ++){
            data->x[i] *= 1.05;
            data->x[i] -= 1;

            data->y[i] *= 1.05;
            data->y[i] -= 1;
        }
    }

    data->zoom += 0.05;

    this->isloading = true;
    this->iszoomin = false;
}
