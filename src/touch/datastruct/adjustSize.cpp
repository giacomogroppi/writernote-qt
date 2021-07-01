#include "datastruct.h"

/*
 * la funzione controlla che non si sia delle parti
 * del pixmap fuori dal foglio, in caso scala tutto
 * di quanto basta.
 *
 * ritorna false se anche con uno spostamento non siamo
 * riusciti a coprire la parte di pixmap fuori dal
 * foglio
*/
bool datastruct::adjustHeight(const uint height,
                              const bool controllRepo)
{
    if(controllRepo)
        controllForRepositioning();

    QPointF __point(0.0, 0.0);
    const point_s * __first = &m_point.first();

    double __traslation;

    __point.setY(biggery());

    if(__point.y() < height){
        __traslation = double(height) - __point.y();

        if(__first->m_y + __traslation > 0)
            return false;

        __point.setY(__traslation);
        scala_all(__point);

        __point.setY(biggery());
        if(__point.y() < height)
            return false;
    }
    return true;
}

/*
 * la funzione viene chiamata quando si zoom diminuendo lo zoom
 * per evitare problemi si controlla che il pixel più grosso all'interno della
 * pagina sia fuori dal pixmap, in larghezza, in caso sia dentro, si verifica se
 * è possibile spostare tutti i dati a destra, se si si sposta e si rifà il
 * controllo che siano fuori, in caso contrario si fa il return di false e
 * bisogna rifare il pixmap
*/
bool datastruct::adjustWidth(const uint width,
                             const bool controllRepo){
    if(controllRepo)
        controllForRepositioning();

    const point_s *__point = & m_point.first();
    QPointF __t(0.0, 0.0);

    scala_all();

    double __translation = biggerx();

    if(__translation < width){
        __translation = width - __translation;
        if(__point->m_x + __translation > 0)
            goto make;

        __t.setX(__translation);
        this->scala_all(__t);

        if(biggerx() < width)
            goto make;

    }

    goto not_make;


    make:
    /* we need to make the pixmap */
    restoreLastTranslation();
    return false;


    not_make:
    /* we don't need to make a new pixmap */
    restoreLastTranslation();
    return true;
}

/*
 * la funzione ritorna adjustWidth
 * the function consider the fact that the
 * height of one sheet is bigger than the width
*/
bool datastruct::adjustAll(const uint width,
                           const uint height){
    return adjustWidth(width, false) || adjustHeight(height, true);
}
