#include "../tabletcanvas.h"
#include <QDebug>

void TabletCanvas::zoomon(QPaintEvent *event){
    if(!this->iszoomon) return;

    if(data->zoom <= 0.0)
        return;

    if(m_pixmap.width() == width())
        this->initPixmap(1);

    else{
        int i, len = data->x.length();

        for(i=0; i < len; i ++){
            /*if(data->x.at(i) < posizionezoom_puntof.x())
                data->x[i] /= 1.05;
            else if(data->x.at(i) > posizionezoom_puntof.x())
                data->x[i] *= 1.05;

            if(data->y.at(i) < posizionezoom_puntof.y())
                data->y[i] /= 1.05;
            else if(data->y.at(i) > posizionezoom_puntof.y())
                data->y[i] *= 1.05;*/
            data->x[i] /= 1.05;
            data->x[i] += 1;

            data->y[i] /= 1.05;
            data->y[i] += 1;
        }
    }

    data->zoom -= 0.05;

    this->isloading = true;
    this->iszoomon = false;
}
