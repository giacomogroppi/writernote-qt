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

    double y;
    const QPointF first = getPointFirstPage();
    double traslation;

    y = biggery();

    if(y < height){
        traslation = double(height) - y;

        if(first.y() + traslation > 0)
            return false;

        y = traslation;
        scala_all(QPointF(0, y));

        y = biggery();
        if(y < height)
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

    const QPointF point = this->getPointFirstPage();
    QPointF __t(0.0, 0.0);

    scala_all();

    double __translation = biggerx();

    if(__translation < width){
        __translation = width - __translation;
        if(point.x() + __translation > 0)
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
