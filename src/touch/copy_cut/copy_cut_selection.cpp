#include "copy_cut_selection.h"
#include "../datastruct/datastruct.h"
#include "../../utils/common_script.h"

copy::copy(QObject *parent) : QObject(parent)
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

/*
 * when this function is called the list of
 * previously saved points will be lost.
 *
 *
 * TODO: implement a 'history' of copies and points.
 * In such a way that the user can save by
 * copying or cutting, multiple strokes.
*/
void copy::selection(datastruct &data, const QList<int> &id, int __flags)
{
    const page *page;
    const QRectF sizeData = data.get_size_area(id);
    this->flags = 0;

    for(int counterPage = data.lengthPage() - 1; counterPage >= 0; counterPage --){
        page = &data.at(counterPage);
        for(int counterStroke = page->lengthStroke() - 1; counterStroke >= 0; counterStroke --){
            const stroke &currentStroke = page->atStroke(counterStroke);

            if(IS_PRESENT_IN_LIST(id, currentStroke.getId())){
                stroke tmp = currentStroke;
                this->m_stroke.append(tmp);

                switch (__flags) {
                case SELECTION_FLAGS_COPY:{
                    break;
                }
                case SELECTION_FLAGS_CUT:{
                    data.at_mod(counterPage).removeAt(counterStroke);
                    counterStroke ++;
                    break;
                }
                default:
                    Q_ASSERT_X(false, "copy::selection", "Flag missing");
                }
            }
        }
    }

    adjustData(sizeData);

    if(flags & FLAG_COPY_SOME_THING_COPY){
        if(__flags == SELECTION_FLAGS_COPY)
            flags &= ~(FLAG_CUT);
        else
            flags &= FLAG_CUT;
    }
}

bool copy::isSomeThingCopy()
{
    return this->flags & FLAG_COPY_SOME_THING_COPY;
}

void copy::adjustData(const QRectF &areaData)
{
    point_s *point;
    int i = this->m_stroke.length() - 1;
    for(; i >= 0; i--){
       point = &m_stroke[i].at_mod(i);

       point->m_x -= areaData.x();
       point->m_y -= areaData.y();

    }
}
