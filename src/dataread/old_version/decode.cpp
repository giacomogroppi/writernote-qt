#include "../xmlstruct.h"

void xmlstruct::decode(datastruct *doc){
    uint i;
    const uint len = doc->length();
    const struct point_s * const first = doc->firstPoint();
    const double deltay = first->m_y;
    const double deltax = first->m_x;

    Q_UNUSED(deltax);
    Q_UNUSED(deltay);

    for(i=0; i<len; ++i){

    }

}
