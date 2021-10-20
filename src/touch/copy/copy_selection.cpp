#include "copy_selection.h"

copy::copy(QObject *parent) : QObject(parent)
{
    this->m_data = new datastruct(nullptr, nullptr);
}

copy::~copy()
{
    delete this->m_data;
}

void copy::copy_selection(datastruct *data, QPointF &topleft, QPointF &bottonright){
    if(data->isempty())
        return;

    this->m_data->reset();

    uint i, len, k;
    const uint lenPage = data->lengthPage();
    /*double x1, x2, y1, y2;

    x1 = topleft.x();
    x2 = bottonright.x();

    y1 = topleft.y();
    y2 = bottonright.y();*/

    const point_s *__point;
    const page *page;

    for(k=0; k<lenPage; ++k){
        page = data->at(k);
        len = page->length();
        for(i=0; i<len; i++){
            __point = page->at(i);

            if(data->isinside(topleft, bottonright, __point)){
                m_data->append(__point);
            }

        }
    }
}

bool copy::isSomeThingCopy()
{
    return !this->m_data->isempty();
}
