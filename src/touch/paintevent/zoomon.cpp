#include "../tabletcanvas.h"
#include <QDebug>

void TabletCanvas::zoomon(QPaintEvent *event){
    if(!this->iszoomon) return;

    this->m_pixmap.fill(Qt::white);

    if(m_pixmap.width() < width())
        this->initPixmap(1);
    else{
        /* non c'è bisogno di cambiare il size del m_pixman in quanto è già al massimo */
        int i, len = data->x.length();

        for(i=0; i < len; i ++){
            if(data->x.at(i) < posizionezoom_puntof.x())
                data->x[i] += 1;
            else if(data->x.at(i) > posizionezoom_puntof.x())
                data->x[i] += -1;
        }

        for(i=0; i < len; i++)
            if(data->y.at(i) < posizionezoom_puntof.y())
                data->y[i] /= 1.01;
            else if(data->y.at(i) > posizionezoom_puntof.y())
                data->y[i] *= 0.98;
    }

    this->isloading = true;
    this->iszoomon = false;
}
