#include "reduce_size.h"
#include "../datastruct/datastruct.h"

static unsigned int howMuchPoint(datastruct *data);

/*
 * every time the user wants to decrease
 * the accuracy of the data, we cut
 * half of the points.
*/

size_t reduce_size::calculate_min_size(datastruct *data){
    size_t __size;

    __size = howMuchPoint(data);

    return current_size(data) - ((__size/2) * sizeof(data->m_point.first()));
}

size_t reduce_size::current_size(datastruct *data){
    size_t size;

    if(data->isempty())
        return 0;

    size = sizeof(data->m_point.first()) * data->m_point.length();

    size += sizeof(data->posizionefoglio.first()) * data->posizionefoglio.length();

    size += sizeof(data->zoom);

    return size;
}

void reduce_size::decrese(datastruct *data){
    unsigned int i, len;
    len = data->length();

    const point_s *__point;
    bool __cont = false;

    if(!len){
        return;
    }

    for(i=0; i<len; ++i){
        __point = & data->m_point.at(i);

        if(!datastruct::isIdUser(__point)){
            /* we CAN'T remove the point we draw */

            ++i;
            continue;
        }

        if(!data->needtochangeid(i)){
            /* we CAN'T remove the first point or the last one */
            continue;
        }

        if(__cont){
            data->m_point.removeAt(i);
            --len;
        }

        __cont = (__cont) ? 0 : 1;

    }


}

static unsigned int __howReduce(datastruct *data,
                      unsigned int *i,
                      unsigned int len){

    unsigned int count;
    int idtratto = data->m_point.at(*i).idtratto;

    const point_s * __point;

    for(count = 0; *i<len; ++(*i)){
        __point = & data->m_point.at(*i);
        if(__point->idtratto != idtratto
                || !__point->isIdUser())
            break;
        count ++;
    }
    return count;
}

static unsigned int howReduce(datastruct *data,
                                unsigned int *i,
                                unsigned int len){

    unsigned int __m;
    __m = __howReduce(data, i, len);


    /*
     * we can't remove the first
     * and the end point
    */

    return (__m > 2) ? (__m-2) : 0;
}

/*
 * the function return the number of point we can
 * remove
*/
static unsigned int howMuchPoint(datastruct *data){
    unsigned int i, len, totalPointToRemove = 0;
    len = data->length();


    for(i=0; i<len; ++i){
        totalPointToRemove += howReduce(data, &i, len);
    }

    return totalPointToRemove;
}
