#include "datastruct.h"

void datastruct::adjustHeight(unsigned int height, bool controllRepo)
{
    if(controllRepo)
        controllForRepositioning();

    QPointF __point;
    __point.setY(biggery());
    __point.setX(0.0);

    if(__point.y() < height){
        __point.setY(height - __point.y());
        scala_all(__point);
    }
}

/*
 * la funzione viene chiamata quando si zoom diminuendo lo zoom
 * per evitare problemi si controlla che il pixel più grosso all'interno della
 * pagina sia fuori dal pixmap, in caso sia dentro, si verifica se
 * è possibile spostare tutti i dati a destra, se si si sposta e si rifà il
 * controllo che siano fuori, in caso contrario si fa il return di false e
 * bisogna rifare il pixmap
*/
bool datastruct::adjustWidth(unsigned int width,
                             bool controllRepo,
                             bool __not_set){
    if(controllRepo)
        controllForRepositioning();

    const point_s *__point = & m_point.first();
    QPointF __t(0.0, 0.0);
    double __x = biggerx();

    if(__x <= width){
        if(__not_set){
            __t.setX(__point->m_x);

            datastruct::inverso(__t);
            this->scala_all(__t);

            return adjustWidth(width,
                               controllRepo,
                               true);
        }else{
            return false;
        }
    }

    return true;

}

void datastruct::adjustAll(unsigned int width,
                           unsigned int height){
    adjustHeight(height, true);
    adjustWidth(width, false);
}
