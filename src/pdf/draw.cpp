#include "topdf.h"

/* the function draw the pixel into the painter  */
void topdf::draw( QPainter *painter, double m){
    int i_, len;

    len = this->data->x.length();

    for(i_ = 0; i_ < len; i_++)
    {
        if(this->data->y.at(i_) < (double)NUMEROPIXELORIZZONALI && this->data->y.at(i_) >= 0){
            /* se cambio il tratto non disegna ma lo carica in lastpoint solamente */
            if(data->idtratto.at(i_) == -1){
                /* carica i punti della pagina */
                updateBrush_load(data->pressure.at(i_), data->color.at(i_));

                painter->setPen(this->penna);
                painter->drawLine(data->x.at(i_)*m, data->y.at(i_)*m
                                  , data->x.at(i_ + 1)*m, data->y.at(i_ + 1)*m);

                /* nello step successivo deve caricare il punto dopo -> non il seguente */
                i_++;
            }
            else if(i_ && this->data->idtratto.at(i_) == this->data->idtratto.at(i_ - 1)
                    && data->y.at(i_) != 1.00
                    && data->y.at(i_) != (double)NUMEROPIXELORIZZONALI){
                this->updateBrush_load(data->pressure.at(i_), data->color.at(i_));

                painter->setPen(this->penna);
                painter->drawLine(this->lastPoint.pos*m,
                              QPointF(this->data->x.at(i_)*m, this->data->y.at(i_)*m));
            }


            lastPoint.pos.setX(this->data->x.at(i_));
            lastPoint.pos.setY(this->data->y.at(i_));
        }
    }
}
