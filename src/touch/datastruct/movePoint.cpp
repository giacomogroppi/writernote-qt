#include "datastruct.h"

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
