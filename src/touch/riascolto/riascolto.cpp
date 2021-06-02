#include "../tabletcanvas.h"

#include <QPainter>

void TabletCanvas::riascolto(int posizione){
    qDebug() << "Riascolto : " << posizione;
    this->m_pos_ris = posizione;
    update();
}
