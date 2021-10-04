#include "datastruct.h"

/*
 * la funzione trasla tutti i punti che sono nel QRectF
 * al punto QPointF &.
 * che sarebbe il punto di tocco dell'utente sullo schermo
 *
 * the function return true if there are same point
 * that he had traslated
 * so we need to update all
*/
void datastruct::MovePoint(const QList<int> &id, const QPointF &__t){
    uint len, i;
    int __last_id, __pos;
    point_s *__point;

    len = length();

    for(i=0; i<len; ++i){
        __pos = id.indexOf(m_point.at(i).idtratto);

        if(__pos != -1){
            __last_id = m_point.at(i).idtratto;

            for(; i<len && m_point.at(i).idtratto == __last_id; i++){
                __point = at_mod(i);

                __point->m_x += __t.x();
                __point->m_y += __t.y();
            }
            --i;

        }
    }
}

bool datastruct::MovePoint(QRectF &rect, QPointF __touch){
    uint i, len;

    const point_s * __point;
    QList<int> __id;

    QPointF topleft, bottonright;
    topleft = rect.topLeft();

    bottonright = rect.bottomRight();

    len = m_point.length();

    for(i=0; i<len; i++){
        __point = & m_point.at(i);
        if(this->isinside(topleft, bottonright, __point)){
            if(__id.indexOf(__point->idtratto) == -1)
                __id.append(__point->idtratto);
        }
    }

    /* there is not id to move */
    if(!__id.length())
        return false;

    this->MovePoint(__id, __touch);

    return true;
}
