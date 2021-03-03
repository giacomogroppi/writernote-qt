#include "../tabletcanvas.h"

#include <QPainter>

/* wheel of mouse */
void TabletCanvas::ismoving_f(){
    unsigned int len, i;

    len = data->datatouch->m_point.length();

    for(i=0; i < len; i++)
    {
        data->datatouch->m_point.operator[](i).m_x += this->ismoving.deltax;
        data->datatouch->m_point.operator[](i).m_y += this->ismoving.deltay;
    }


    len = data->datatouch->posizionefoglio.length();
    for(i=0; i< len; i++)
        data->datatouch->posizionefoglio[i] += ismoving.deltay;

    this->isloading = true;
    update();
}
