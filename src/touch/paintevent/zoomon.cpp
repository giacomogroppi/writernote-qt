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
        this->initPixmap();

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

    int i, len, delta, deltaf;
    len = datastruct_->x.length();

    for(i=0; i < len; i++)
        if(datastruct_->idtratto.at(i) == -1)
            break;

    delta = datastruct_->x.at(i);
    deltaf = datastruct_->x.at(i+1) - m_pixmap.width();

    if(deltaf < 0)
        deltaf = 0;

    if(delta <= 0 && !deltaf)
        return;

    if(delta > 0 && deltaf > 0){
        /* in questo caso vuol dire che la pagina è sia più piccola a sinistra che a destra
           quindi deve spostare tutto a sinistra e ridimensionare il pixmal della dimensione
           corrente
           -> altrimenti si può scrivere fuori dal foglio.
        */

        for(i=0; i<len; i++){
            datastruct_->x[i] -= delta;
        }

        this->initPixmap();
        return;
    }

    for(i=0; i<len; i++){
        datastruct_->x[i] -= (delta + deltaf);
    }


}
