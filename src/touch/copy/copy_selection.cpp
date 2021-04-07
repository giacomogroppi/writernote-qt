#include "copy_selection.h"

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


        if(data->isinside(topleft, bottonright, __point)){
            this->m_data.m_point.append(*__point);
        }

    }

    adjastTranslation();

}

/*
 * the function past the last copy of datastruct
*/
void copy::past_selection(datastruct *data_past, QPointF &point_past)
{
    unsigned int i, len;

    len = this->m_data.m_point.length();

    const point_s * __point;
    point_s __append;

    for(i=0; i<len; i++){
        __point = & m_data.m_point.at(i);

        if(!data_past->isAvailable(__point->idtratto)){
            m_data.changeId(i, data_past->maxId()+1);
        }

        memcpy(&__append, __point, sizeof(__append));

        /* we move the point from (0, 0) to point_past */
        __append.m_y += point_past.y();
        __append.m_x += point_past.x();

        data_past->m_point.append(__append);

    }

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
