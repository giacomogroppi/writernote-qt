#include "topdf.h"

#define C(x) x->datatouch->m_point
#include "../utils/color/setcolor.h"
/*
 * TODO: -> remove this function and use tablet::load(QPainter &)
 * only difference it pass the size_v, and size_o, and the parameter
 * m [double]
*/

/* the function draw the pixel into the painter  */
void topdf::draw( QPainter &painter,
                  double m,
                  int size_orizzontale,
                  int size_verticale,
                  double *y_last){

    m_canvas->load(painter,
                   m,
                   size_orizzontale,
                   size_verticale,
                   y_last);

}
