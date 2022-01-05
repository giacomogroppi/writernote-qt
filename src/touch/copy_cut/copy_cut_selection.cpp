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

static void __single(const QVector<int> &pos, page &data, QList<stroke> & append_data, int __flags){
    for(int i = 0; i < pos.length(); i++){
        const stroke &currentStroke = data.atStroke(pos.at(i));

        stroke tmp(currentStroke);
        append_data.append(tmp);

        switch (__flags) {
        case SELECTION_FLAGS_COPY:{
            break;
        }
        case SELECTION_FLAGS_CUT:{
            data.removeAt(pos.at(i));
            break;
        }
        case SELECTION_FLAGS_PASTE:{
            break;
        }
        default:
            Q_ASSERT_X(false, "copy::selection", "Flag missing");
        }
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
*/
void copy::selection(
        datastruct                  &data,
        const QList<QVector<int> >  &id,
        int                         page_base,
        int                         __flags,
        QList<int>                  &page_mod,
        const QPointF               &pointTouch)
{
    page *page;
    QRectF sizeData;
    QPointF tmpPoint;
    int lenList;

    lenList = id.length();

    if(__flags == SELECTION_FLAGS_PASTE){
        tmpPoint = data.adjustPoint(pointTouch);
        this->managePaste(data, tmpPoint);

        if(this->isSomeThingCut()){
            flags = 0;
            this->m_stroke.clear();
        }

        return;
    }

    m_stroke.clear();
    sizeData = data.get_size_area(id, page_base);
    this->flags = 0;

    if(flags & SELECTION_FLAGS_CUT){
        int i;

        for(i = 0; i < lenList; i++){
            if(likely(!id.at(i).isEmpty())){
                page_mod.append(page_base + i);
            }
        }
    }

    for(int i = 0; i < lenList; i ++){
        page = &data.at_mod(page_base + i);
        const QVector<int> &mid_list = id.at(i);
        __single(mid_list, *page, m_stroke, __flags);
    }

    adjustData(sizeData.topLeft());

    if(!this->isEmpty()){
        if(__flags == SELECTION_FLAGS_COPY)
            flags &= ~(FLAG_CUT);
        else
            flags &= FLAG_CUT;
    }
}

void copy::adjustData(const QPointF &offset)
{
    int counterStroke = this->m_stroke.length() - 1;
    const QPointF &inverso = datastruct::inverso(offset);

    for(; counterStroke >= 0; counterStroke --){
        m_stroke.operator[](counterStroke).scale(inverso);
    }
}
