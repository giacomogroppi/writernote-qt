#include "topdf.h"

/* the function draw the pixel into the painter  */
void topdf::draw( QPainter *painter, double m, int size_orizzontale, int size_verticale){
    int i_, len;

    len = data->datatouch->x.length();

    QColor coloretemp;

    for(i_ = 0; i_ < len; i_++)
    {
        coloretemp.setRgb(data->datatouch->color.at(i_).colore[0],
                data->datatouch->color.at(i_).colore[1],
                data->datatouch->color.at(i_).colore[2],
                data->datatouch->color.at(i_).colore[3]);

        if(data->datatouch->y.at(i_)*m < (size_verticale) && data->datatouch->y.at(i_) >= 0){
            /* se cambio il tratto non disegna ma lo carica in lastpoint solamente */
            if(data->datatouch->idtratto.at(i_) == -1){
                updateBrush_load(data->datatouch->pressure.at(i_), coloretemp);

                painter->setPen(this->penna);
                painter->drawLine(data->datatouch->x.at(i_)*m, data->datatouch->y.at(i_)*m
                                  , data->datatouch->x.at(i_ + 1)*m, data->datatouch->y.at(i_ + 1)*m);

                /* nello step successivo deve caricare il punto dopo -> non il seguente */
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
            lastPoint.pos.setY(data->datatouch->y.at(i_));
        }
    }
}
