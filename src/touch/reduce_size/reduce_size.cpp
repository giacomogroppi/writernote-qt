#include "reduce_size.h"
#include "../datastruct/datastruct.h"

/*
 * every time the user wants to decrease
 * the accuracy of the data, we cut
 * half of the points.
*/

size_t reduce_size::calculate_min_size(datastruct *data){
    size_t __size;

    __size = current_size(data);

    return __size/2;

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
    }


}
