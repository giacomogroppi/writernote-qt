#include "topdf.h"

#define C(x) x->datatouch->m_point
#include "../utils/color/setcolor.h"
/*
 * TODO: -> remove this function and use tablet::load(QPainter &)
 * only difference it pass the size_v, and size_o, and the parameter
 * m [double]
*/

/* the function draw the pixel into the painter  */
void topdf::draw( QPainter *painter,
                  double m,
                  int size_orizzontale,
                  int size_verticale,
                  double *y_last){

    m_canvas->load(painter,
                   m,
                   size_orizzontale,
                   size_verticale,
                   y_last);

/*    int i_, len;


    len = C(data).length();

    QColor coloretemp;

    for(i_ = 0; i_ < len; i_++)
    {
        coloretemp = setcolor(&C(data).at(i_).m_color);

        if(C(data).at(i_).m_y*m < (size_verticale) && C(data).at(i_).m_y >= 0){

            if(C(data).at(i_).idtratto == -1){
                updateBrush_load(C(data).at(i_).m_pressure, coloretemp);

                painter->setPen(this->penna);
                painter->drawLine(C(data).at(i_).m_x*m, C(data).at(i_).m_y*m
                                  , C(data).at(i_ + 1).m_x*m, C(data).at(i_ + 1).m_y*m);


                i_++;
            }
            else if(i_ && C(data).at(i_).idtratto == C(data).at(i_ - 1).idtratto
                    && C(data).at(i_).m_y != 1.00
                    && C(data).at(i_).m_y != (double)size_orizzontale){

                this->updateBrush_load(C(data).at(i_).m_pressure, coloretemp);

                painter->setPen(this->penna);
                painter->drawLine(this->lastPoint.pos*m,
                              QPointF(C(data).at(i_).m_x*m, C(data).at(i_).m_y*m));
            }


            lastPoint.pos.setX(C(data).at(i_).m_x);
            lastPoint.pos.setY(C(data).at(i_).m_y);

            *y_last = C(data).at(i_).m_y;
*/
        /*
         * last data struct
        */
        /*
        coloretemp.setRgb(data->datatouch->color.at(i_).colore[0],
                data->datatouch->color.at(i_).colore[1],
                data->datatouch->color.at(i_).colore[2],
                data->datatouch->color.at(i_).colore[3]);

        if(data->datatouch->y.at(i_)*m < (size_verticale) && data->datatouch->y.at(i_) >= 0){

            if(data->datatouch->idtratto.at(i_) == -1){
                updateBrush_load(data->datatouch->pressure.at(i_), coloretemp);

                painter->setPen(this->penna);
                painter->drawLine(data->datatouch->x.at(i_)*m, data->datatouch->y.at(i_)*m
                                  , data->datatouch->x.at(i_ + 1)*m, data->datatouch->y.at(i_ + 1)*m);


                i_++;
            }
            else if(i_ && data->datatouch->idtratto.at(i_) == data->datatouch->idtratto.at(i_ - 1)
                    && data->datatouch->y.at(i_) != 1.00
                    && data->datatouch->y.at(i_) != (double)size_orizzontale){
                this->updateBrush_load(data->datatouch->pressure.at(i_), coloretemp);

                painter->setPen(this->penna);
                painter->drawLine(this->lastPoint.pos*m,
                              QPointF(data->datatouch->x.at(i_)*m, data->datatouch->y.at(i_)*m));
            }


            lastPoint.pos.setX(data->datatouch->x.at(i_));
            lastPoint.pos.setY(data->datatouch->y.at(i_));*/
        //}
    //}
}
