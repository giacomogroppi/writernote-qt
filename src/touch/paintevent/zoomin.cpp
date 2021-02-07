#include "../tabletcanvas.h"
#include <QDebug>

/* automenta lo zoom */
/*void TabletCanvas::zoomin(currenttitle_class *datastruct_){
    if(!datastruct_){
        datastruct_ = this->data;

        this->isloading = true;
    }
    if(datastruct_->datatouch->zoom >= 2.0)
        return;

    if(m_pixmap.width() < width())
        this->initPixmap();
    else{
        int i, len = datastruct_->datatouch->x.length();

        for(i=0; i < len; i ++){
            datastruct_->datatouch->x[i] *= 1.05;
            datastruct_->datatouch->x[i] -= 1;

            datastruct_->datatouch->y[i] *= 1.05;
            datastruct_->datatouch->y[i] -= 1;
        }
    }

    data->datatouch->zoom += 0.05;

}*/
