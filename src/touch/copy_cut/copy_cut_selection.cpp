#include "copy_cut_selection.h"
#include "touch/dataTouch/datastruct/datastruct.h"
#include "utils/WCommonScript.h"

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

    datastruct::inverso(tmp);
    this->adjustData(tmp);

    data.append(this->m_stroke, -1);

    if(this->isSomeThingCopy()){
        datastruct::inverso(tmp);
        this->adjustData(tmp);
    }
}

QRect copy::get_size_area(const QList<QList<Stroke>> &data)
{
    QRect size_area(0, 0, 0, 0);

    for(const auto &__list : data){

        const auto tmp = Page::get_size_area(__list, 0, __list.length());

        size_area = datastruct::get_bigger_rect(tmp, size_area);
    }

    return size_area;
}

void copy::__single(const QList<Stroke *> &from, QList<Stroke *> &append_data)
{
    for (const auto *currentStroke : qAsConst(from)) {
        append_data.append(currentStroke->clone());
    }
}

/**
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
 *
 * Il punto si riferisce già alla struttura dati
 * e non al widget
*/
int copy::selection(
        datastruct                  &data,
        const QList<QList<Stroke*>>  &stroke,
        int                         __flags,
        const QPointF               &pointTouch)
{
    QRect sizeData;
    QPointF tmpPoint;
    int lenList, i;

    lenList = stroke.length();

    if(__flags == SELECTION_FLAGS_PASTE){
        //tmpPoint = data.adjustPoint(pointTouch);
        tmpPoint = pointTouch;
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

    for(i = 0; i < lenList; i ++){
        __single(stroke.at(i), m_stroke);
    }

    adjustData(pointTouch);

    if(unlikely(this->isEmpty())){
        return 0;
    }

    if(__flags == SELECTION_FLAGS_COPY)
        flags &= ~(FLAG_CUT);
    else{
        flags |= FLAG_CUT;

        // we have modify the data so we need
        // to update the image
        data.drawIfInside(sizeData);
    }

    return (int)this->isSomeThingCut();
}

void copy::adjustData(const QPointF &offset)
{
    int counterStroke = this->m_stroke.length() - 1;
    const QPointF &inverso = datastruct::inverso(offset);

    for(; counterStroke >= 0; counterStroke --){
        m_stroke.operator[](counterStroke).scale(inverso);
    }
}
