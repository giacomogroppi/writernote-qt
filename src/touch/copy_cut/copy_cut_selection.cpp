#include "copy_cut_selection.h"
#include "touch/datastruct/datastruct.h"
#include "utils/common_script.h"

copy::copy()
{

}

copy::~copy()
{

}

void copy::reset()
{
    this->flags = 0;
    this->m_stroke.clear();
}

void copy::managePaste(
        datastruct      &data,
        const QPointF   &pointTouch)
{
    QPointF tmp = pointTouch;
    int i;
    int tmpId = -1;

    i = this->m_stroke.length() - 1;
    datastruct::inverso(tmp);
    this->adjustData(tmp);

    for(; i >= 0; i --){
        stroke &stroke = m_stroke.operator[](i);

        /* in case we cut */
        if(data.isAvailable(stroke.getId()))
            continue;

        if(tmpId < 0)
            tmpId = data.maxId() + 1;

        stroke.setId(tmpId);
        tmpId += 1;
    }

    data.append(this->m_stroke, -1);
    if(this->isSomeThingCopy()){
        datastruct::inverso(tmp);
        this->adjustData(tmp);
    }
}

QRect copy::get_size_area(const QList<QList<stroke>> &data)
{
    QRect size_area(0, 0, 0, 0);
    int __tmp = 0;
    for(const auto &__list : data){

        const auto tmp = page::get_size_area(__list, 0, __list.length());

        size_area = datastruct::get_bigger_rect(tmp, size_area);
        __tmp ++;
    }

    return size_area;
}

void copy::__single(const QList<stroke> &from, QList<stroke> &append_data)
{
    int len = from.length();
    for(int i = 0; i < len; i++){
        const stroke &currentStroke = from.at(i);

        stroke tmp(currentStroke);
        append_data.append(tmp);
    }
}

/*
 * when this function is called the list of
 * previously saved points will be lost.
 *
 *
 * TODO: implement a 'history' of copies and points.
 * In such a way that the user can save by
 * copying or cutting, multiple strokes.
 *
 * Return 1 if we need to remove the stroke
 * pass from the list (if we are in copy mode)
*/
int copy::selection(
        datastruct                  &data,
        const QList<QList<stroke>>  &stroke,
        int                         __flags,
        const QPointF               &pointTouch)
{
    QRectF sizeData;
    QPointF tmpPoint;
    int lenList;

    lenList = stroke.length();

    if(__flags == SELECTION_FLAGS_PASTE){
        tmpPoint = data.adjustPoint(pointTouch);
        this->managePaste(data, tmpPoint);

        if(this->isSomeThingCut()){
            flags = 0;
            this->m_stroke.clear();
        }

        return 0;
    }

    m_stroke.clear();
    sizeData = get_size_area(stroke);

    this->flags = 0;

    for(int i = 0; i < lenList; i ++){
        __single(stroke.at(i), m_stroke);
    }

    adjustData(sizeData.topLeft());

    if(!this->isEmpty()){
        if(__flags == SELECTION_FLAGS_COPY)
            flags &= ~(FLAG_CUT);
        else
            flags |= FLAG_CUT;
    }

    return (int)this->isSomeThingCut();
}

void copy::adjustData(const QPointF &offset)
{
    int counterStroke = this->m_stroke.length() - 1;
    const QPointF &inverso = datastruct::inverso(offset);

    for(; counterStroke >= 0; counterStroke --){
        m_stroke.operator[](counterStroke).scale(inverso, 0);
    }
}
