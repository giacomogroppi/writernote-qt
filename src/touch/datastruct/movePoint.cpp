#include "datastruct.h"

/*
 * la funzione trasla tutti i punti che sono nel QRectF
 * al punto QPointF &.
 * che sarebbe il punto di tocco dell'utente sullo schermo
 *
 * the function return true if there are same point
 * that he had traslated
 * so we need to update all
*/

void datastruct::MovePoint(
        const QList<int>    &id,
        const QPointF       &translation,
        QList<int>          *PageModify)
{
    uint counterPage, lenPage;
    uint counterStroke, lenStroke;
    int mod;

    lenPage = this->lengthPage();

    for (counterPage = 0; counterPage < lenPage; counterPage ++){
        page &page = at_mod(counterPage);
        mod = 0;
        lenStroke = page.lengthStroke();

        for(counterStroke = 0; counterStroke < lenStroke; counterStroke ++){
            if(id.indexOf(page.atStroke(counterPage).getId()) != -1){
                mod = 1;
                page.atStrokeMod(counterStroke).movePoint(translation);
            }
        }
        if(mod && PageModify)
            PageModify->append(counterPage);
    }
}

/*QList<int> *datastruct::MovePoint(const QRectF &rect, const QPointF &__touch){
    uint i, len;
    const uint lenPage = this->lengthPage();
    QPointF topleft, bottonright;

    static QList<int> __id;

    __id.clear();

    topleft = rect.topLeft();

    bottonright = rect.bottomRight();


    for(uint k=0; k<lenPage; k++){
        len = at(k).lengthStroke();

        for(i=0; i<len; i++){
            const stroke &stroke = at(k).atStroke(i);

            if(this->isinside(topleft, bottonright, stroke)){
                if(__id.indexOf(stroke.getId()) == -1){
                    __id.append(stroke.getId());
                }
            }
        }
    }

    this->MovePoint(__id, __touch);

    return &__id;
}*/
