#include "../tabletcanvas.h"
#include <QDebug>

/*void TabletCanvas::zoomon(currenttitle_class *datastruct_){
    if(!datastruct_){
        datastruct_ = this->data;
        this->isloading = true;
    }

    if(datastruct_->datatouch->zoom <= 0.0)
        return;

    if(m_pixmap.width() == width())
        this->initPixmap();

    else{
        int i, len = datastruct_->datatouch->x.length();

        for(i=0; i < len; i ++){
            datastruct_->datatouch->x[i] /= 1.05;
            datastruct_->datatouch->x[i] += 1;

            datastruct_->datatouch->y[i] /= 1.05;
            datastruct_->datatouch->y[i] += 1;
        }
    }

    datastruct_->datatouch->zoom -= 0.05;

    int i, len, delta, deltaf;
    len = datastruct_->datatouch->x.length();

    for(i=0; i < len; i++)
        if(datastruct_->datatouch->idtratto.at(i) == -1)
            break;

    delta = datastruct_->datatouch->x.at(i);
    deltaf = datastruct_->datatouch->x.at(i+1) - m_pixmap.width();

    if(deltaf < 0)
        deltaf = 0;

    if(delta <= 0 && !deltaf)
        return;

    if(delta > 0 && deltaf > 0){

        for(i=0; i<len; i++){
            datastruct_->datatouch->x[i] -= delta;
        }

        this->initPixmap();
        return;
    }

    for(i=0; i<len; i++){
        datastruct_->datatouch->x[i] -= (delta + deltaf);
    }


}*/
