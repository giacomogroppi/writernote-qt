#include "datastruct.h"

/*
 * return 1 if there is some
 * point outside the removing page
*/
static uchar hasComponent(uint len,
                          uint i,
                          datastruct *data,
                          double ymin,
                          double ymax,
                          const point_s *__point){
    int id = __point->idtratto;
    for(; i<len && __point->idtratto == id; ++i){
        __point = data->at(i);
        if(__point->m_y < ymin || __point->m_y > ymax)
            return 1;
    }

    return 0;
}

/*
 * return 0 all ok
 * return 1 the page to remove is outsize
*/
uchar datastruct::removePage(uint page)
{
    uint i, len = posizionefoglio.length();
    if(page > len)
        return 1;

    int id;
    double ymin, ymax;
    ymin = posizionefoglio.at(page);

    if(len - 1 > page){
        ymax = posizionefoglio.at(page + 1);
    }else{
        ymax = biggery();
    }

    const point_s * __point;

    for(i=0; i<len; ++i){
        __point = at(i);

        if(__point->m_y > ymin
                && __point->m_y < ymax){
            if(hasComponent(len, i, this, ymin, ymax, __point)){
                while(__point->m_y > ymin || __point->m_y < ymax){
                    m_point.removeAt(i);
                    len -= 1;
                    i -= 1;
                    __point = at(i);
                }

                id = maxId() + 1;
                for(; i<len && (__point->m_y < ymin || __point->m_y > ymax); ++i){
                    m_point.operator[](i).idtratto = id;
                }

            }
        }
    }

    /*
     * if there are many strokes between one
     * page to another it is possible to leave
     * many unused strokes, for this reason
     * we understand which ones are not used
     *  and move them
    */
    reorganize();

    return 0;

}
