#include "../xmlstruct.h"

void xmlstruct::decode(Document *data){
    datastruct *doc = data->datatouch;
    const uint len = doc->length();
    
    if(!len)
        return;
    
    const struct point_s * const first = doc->firstPoint();
    const double deltay = first->m_y;
    const double deltax = first->m_x;

    doc->scala_all(QPointF(-deltax, -deltay));

    doc->setPointFirstPage(QPointF(deltax, deltay));

}
