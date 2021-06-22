#include "../tabletcanvas.h"

#include <QPainter>

/* wheel of mouse */
void TabletCanvas::ismoving_f(){
    data->datatouch->scala_all(ismoving.point, m_from_pdf);

    updatePageCount();

    this->isloading = true;
    update();
}
