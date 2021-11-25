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

    uint i, len, counterPage;
    const uint lenPage = data->lengthPage();
    /*double x1, x2, y1, y2;

    x1 = topleft.x();
    x2 = bottonright.x();

    y1 = topleft.y();
    y2 = bottonright.y();*/


    for(counterPage = 0; counterPage < lenPage; counterPage++){
        const page &page = data->at(counterPage);
        len = page.lengthStroke();
        for(i = 0; i < len; i++){
            const stroke & stroke = page.atStroke(i);

            if(data->isinside(topleft, bottonright, stroke)){
                m_data->appendStroke(stroke);
            }

        }
    }
}

bool copy::isSomeThingCopy()
{
    return !this->m_data->isempty();
}
