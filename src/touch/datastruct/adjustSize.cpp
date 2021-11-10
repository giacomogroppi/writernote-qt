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
void datastruct::adjustHeight(const uint height)
{
    const QPointF point = this->getPointFirstPage();
    QPointF t(0.0, 0.0);

    double y = biggery();

    if(y < height){
        t.setX(height-y);
    }else{ //(x >= width)
        if(point.y() > 0.0)
            t.setX(-point.y());
    }

    scala_all(t, height);
}

/*
 * la funzione viene chiamata quando si zoom diminuendo lo zoom
 * per evitare problemi si controlla che il pixel più grosso all'interno della
 * pagina sia fuori dal pixmap, in larghezza, in caso sia dentro, si verifica se
 * è possibile spostare tutti i dati a destra, se si si sposta e si rifà il
 * controllo che siano fuori, in caso contrario si fa il return di false e
 * bisogna rifare il pixmap
*/
void datastruct::adjustWidth(const uint width)
{
    QPointF point = this->getPointFirstPage();
    QPointF t(0.0, 0.0);
    double biggerX = biggerx();

    if(point.x() < 0.0 && biggerX <= width){
        t.setX(width - biggerX);
    }else{ //(x >= width)
        if(point.x() > 0.0)
            t.setX(-point.x());
    }

    scala_all(t*zoom);

    point = this->getPointFirstPage();
    if(point.x() > 0.0){
        t.setX(-point.x());
        scala_all(t);
    }
}

/*
 * la funzione ritorna adjustWidth
 * the function consider the fact that the
 * height of one sheet is bigger than the width
*/
void datastruct::adjustAll(const uint width,
                           const uint height){
    adjustWidth(width);
    adjustHeight(height);
}

void datastruct::adjustAll(const QSize &size)
{
    this->adjustAll(size.width(), size.height());
}
