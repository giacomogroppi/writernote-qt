#include "copy.h"

copy::copy(QObject *parent) : QObject(parent)
{

}

void copy::copy_selection(datastruct *data, QPointF &topleft, QPointF &bottonright){
    if(data->isempty())
        return;

    this->m_data.reset();

    unsigned i, len;
    /*double x1, x2, y1, y2;

    x1 = topleft.x();
    x2 = bottonright.x();

    y1 = topleft.y();
    y2 = bottonright.y();*/

    const point_s *__point;
    len = data->m_point.length();

    for(i=0; i<len; i++){
        __point = &data->m_point.at(i);


        if(data->isinside(topleft, bottonright, i)){
            this->m_data.m_point.append(*__point);
        }

    }

    adjastTranslation();

}

bool copy::isSomeThingCopy()
{
    return !this->m_data.isempty();
}

void copy::adjastTranslation()
{
    if(this->m_data.isempty())
        return;

    QPointF min;
    min.setX(this->m_data.minx());
    min.setY(this->m_data.miny());


    datastruct::inverso(min);

    /* we suppose that all point have x and y > 0 */

    m_data.scala_all(min);

}
