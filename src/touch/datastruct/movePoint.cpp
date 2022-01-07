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

void datastruct::MovePoint(
        const QList<QVector<int> >  &pos,
        cint                        base,
        const QPointF               &translation)
{
    int i, len = pos.length();

    for(i = 0; i < len; i++){
        const QVector<int> & ref = pos.at(i);

        MovePoint(ref, base + i, translation);
    }
}

/* the list can be not order */
void datastruct::MovePoint(
        const QVector<int>  &pos,
        cint                __page,
        const QPointF       &translation)
{
    page &page = at_mod(__page);
    for(const auto &index : qAsConst(pos)){
        stroke & stroke = page.atStrokeMod(index);
        stroke.movePoint(translation);
    }
}

void datastruct::MovePoint(QList<stroke> &stroke, const QPointF &translation, int flag)
{
    int i = stroke.length() - 1;
    if(flag & DATASTRUCT_MUST_TRASLATE_PATH){
        flag = STROKE_MUST_TRASLATE_PATH;
    }

    for(; i >= 0; i --){
        stroke.operator[](i).scale(translation, flag);
    }

}
