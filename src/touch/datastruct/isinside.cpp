#include "datastruct.h"

bool datastruct::isinside(QPointF &topleft, QPointF &bottonright, unsigned int index)
{
    return isinside(topleft.x(),
                    topleft.y(),
                    bottonright.x(),
                    bottonright.y(),
                    index);
}

bool datastruct::isinside(double x1, double y1, double x2, double y2, unsigned int index)
{
    const point_s *__point;

    __point = &m_point.at(index);

    return __point->m_x > x1
            && __point->m_y > y1
            && __point->m_x < x2
            && __point->m_y < y2
            && isIdUser(__point) ;
}

bool datastruct::isinside(QPointF &topleft, QPointF &bottonright, const point_s *__point){
    return __point->m_x > topleft.x()
            && __point->m_y > topleft.y()
            && __point->m_x < bottonright.x()
            && __point->m_y < bottonright.y()
            && isIdUser(__point);
}

bool datastruct::isinside(QPointF &topleft, QPointF &bottonright, QPointF &point){
    return topleft.x() <= point.x()
            && topleft.y() <= point.y()
            && bottonright.x() >= point.x()
            && bottonright.y() >= point.y();
}
