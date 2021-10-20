#include "datastruct.h"
#include <QDebug>

bool datastruct::isinside(QPointF &topleft, QPointF &bottonright, const uint index, const uint page)
{
    return isinside(topleft.x(),
                    topleft.y(),
                    bottonright.x(),
                    bottonright.y(),
                    index, page);
}

bool datastruct::isinside(double x1, double y1, double x2, double y2, const uint index, const uint page)
{
    QPointF __topleft(x1, y1), bottonright(x2, y2);

    return datastruct::isinside(__topleft, bottonright, at(index, page));
}

bool datastruct::isinside(const QPointF &topleft, const QPointF &bottonright, const point_s *__point){
    return  __point->m_x > topleft.x()
            && __point->m_y > topleft.y()
            && __point->m_x < bottonright.x()
            && __point->m_y < bottonright.y()
            && isIdUser(__point);
}

bool datastruct::isinside(const QPointF &topleft, const QPointF &bottonright, const QPointF &point){
    return  topleft.x() <= point.x()
            && topleft.y() <= point.y()
            && bottonright.x() >= point.x()
            && bottonright.y() >= point.y();
}
