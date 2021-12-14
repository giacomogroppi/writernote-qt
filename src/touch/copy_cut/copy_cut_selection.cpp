#include "copy_cut_selection.h"
#include "../datastruct/datastruct.h"
#include "../../utils/common_script.h"

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

    qDebug() << data.at(0).lengthStroke();


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
    qDebug() << data.at_mod(0).lastMod().getBiggerPointInStroke();
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
        datastruct          &data,
        const QList<int>    &id,
        int                 __flags,
        QList<int>          &page_mod,
        const QPointF       &pointTouch)
{
    const page *page;
    QRectF sizeData;
    QPointF tmpPoint;
    int mod;

    if(__flags == SELECTION_FLAGS_PASTE){
        tmpPoint = data.adjustPoint(pointTouch);
        this->managePaste(data, tmpPoint);

        if(this->isSomeThingCut()){
            flags = 0;
            this->m_stroke.clear();
        }

        return;
    }

    sizeData = data.get_size_area(id);
    this->flags = 0;

    for(int counterPage = data.lengthPage() - 1; counterPage >= 0; counterPage --){
        page = &data.at(counterPage);
        mod = 0;
        for(int counterStroke = page->lengthStroke() - 1; counterStroke >= 0; counterStroke --){
            const stroke &currentStroke = page->atStroke(counterStroke);

            if(IS_PRESENT_IN_LIST(id, currentStroke.getId())){
                stroke tmp(currentStroke);
                this->m_stroke.append(tmp);

                switch (__flags) {
                case SELECTION_FLAGS_COPY:{
                    break;
                }
                case SELECTION_FLAGS_CUT:{
                    data.at_mod(counterPage).removeAt(counterStroke);
                    mod = 1;
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
        if(mod)
            page_mod.append(counterPage);
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
    point_s *point;
    int counterStroke = this->m_stroke.length() - 1;
    for(; counterStroke >= 0; counterStroke --){
        stroke &stroke = m_stroke.operator[](counterStroke);
        int counterPoint = stroke.length() - 1;

        for(; counterPoint >= 0; counterPoint --){
            point = &stroke.at_mod(counterPoint);

            point->m_x -= offset.x();
            point->m_y -= offset.y();
        }


    }
}
