#include "../tabletcanvas.h"

#include <QPainter>

/* funzione che gestisce lo scorrimento con il touch e con la rotella del mouse */
void TabletCanvas::ismoving_f(){
    unsigned int len, i;

    len = data->datatouch->m_point.length();
    //len = data->datatouch->y.length();

    for(i=0; i < len; i++)
    {
        data->datatouch->m_point.operator[](i).m_x += this->ismoving.deltax;
        data->datatouch->m_point.operator[](i).m_y += this->ismoving.deltay;

        /* last data struct */
        /*data->datatouch->y[i] += this->ismoving.deltay;
        data->datatouch->x[i] += this->ismoving.deltax;*/
    }


    len = data->datatouch->posizionefoglio.length();
    for(i=0; i< len; i++)
        data->datatouch->posizionefoglio[i] += ismoving.deltay;

    this->isloading = true;
    update();
}
