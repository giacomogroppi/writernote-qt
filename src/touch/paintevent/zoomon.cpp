#include "../tabletcanvas.h"
#include <QDebug>

void TabletCanvas::zoomon(datastruct *datastruct_){
    if(!this->iszoomon && !datastruct_) return;

    /* if it is called by paintevent */
    if(!datastruct_){
        datastruct_ = this->data;
        this->isloading = true;
        this->iszoomon = false;
    }

    if(datastruct_->zoom <= 0.0)
        return;

    if(m_pixmap.width() == width())
        this->initPixmap(1);

    else{
        int i, len = datastruct_->x.length();

        for(i=0; i < len; i ++){
            datastruct_->x[i] /= 1.05;
            datastruct_->x[i] += 1;

            datastruct_->y[i] /= 1.05;
            datastruct_->y[i] += 1;
        }
    }

    datastruct_->zoom -= 0.05;

}
