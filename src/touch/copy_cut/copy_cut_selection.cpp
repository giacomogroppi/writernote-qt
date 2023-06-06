#include "copy_cut_selection.h"
#include "touch/dataTouch/datastruct/DataStruct.h"
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
        DataStruct      &data,
        const PointF   &pointTouch)
{
    PointF tmp = pointTouch;

    DataStruct::inverso(tmp);
    this->adjustData(tmp);

    data.append(this->m_stroke, -1);

    if(this->isSomeThingCopy()){
        DataStruct::inverso(tmp);
        this->adjustData(tmp);
    }
}

RectF copy::get_size_area(const WListFast<WListFast<std::shared_ptr<Stroke>>> &data)
{
    RectF size_area(0, 0, 0, 0);

    for(const auto &sub : data){

        const auto tmp = Page::get_size_area(sub, 0, sub.size());

        size_area = DataStruct::joinRect(tmp, size_area);
    }

    return size_area;
}

void copy::single(
                const WListFast<    std::shared_ptr<Stroke>> &from,
                WListFast<          std::shared_ptr<Stroke>> &append_data)
{
    for (const auto &currentStroke : std::as_const(from)) {
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
        DataStruct                  &data,
        const WListFast<WListFast<std::shared_ptr<Stroke>>>  &stroke,
        int                         __flags,
        const PointF               &pointTouch)
{
    RectF sizeData;
    PointF tmpPoint;
    int lenList, i;

    lenList = stroke.size();

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
        single(stroke.at(i), m_stroke);
    }

    adjustData(pointTouch);

    if(un(this->isEmpty())){
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

void copy::adjustData(const PointF &offset)
{
    using namespace WCommonScript;
    const PointF &inverso = DataStruct::inverso(offset);

    for_each(this->m_stroke, [&inverso](std::shared_ptr<Stroke> d) {
        d->scale(inverso);
    });
}
