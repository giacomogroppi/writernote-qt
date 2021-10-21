#include "reduce_size.h"
#include "../datastruct/datastruct.h"
#include "../datastruct/page.h"

static uint howMuchPoint(datastruct *data);

/*
 * every time the user wants to decrease
 * the accuracy of the data, we cut
 * half of the points.
*/

size_t reduce_size::calculate_min_size(datastruct *data){
    size_t __size;

    __size = howMuchPoint(data);

    return current_size(data) - ((__size/2) * sizeof(point_s));
}

size_t reduce_size::current_size(datastruct *data){
    size_t size;

    if(data->isempty())
        return 0;

    size = data->getSizeOne() * data->lengthPoint();

    size += sizeof(double)*2; /* first point --> x and y */

    size += sizeof(data->zoom);

    return size;
}

void reduce_size::decrese(datastruct *data){
    uint i, pageCounter, len;
    const uint lenPage = data->lengthPage();
    page *page;
    const point_s *__point;
    bool __cont = false;

    if(!lenPage){
        return;
    }
    for(pageCounter=0; pageCounter<lenPage; pageCounter++){
        page = data->at_mod(pageCounter);

        len = page->length();
        for(i=0; i<len; ++i){
            __point = page->at(i);

            if(!datastruct::isIdUser(__point)){
                /* we CAN'T remove the point we draw */

                ++i;
                continue;
            }

            if(!page->needtochangeid(i)){
                /* we CAN'T remove the first point or the last one */
                continue;
            }

            if(__cont){
                page->removeAt(i);
                --len;
            }

            __cont = (__cont) ? 0 : 1;

        }
    }


}

static uint __howReduce(uint &i,
                        const page *page, const uint lenPoint){

    uint count;
    const int idtratto = page->at(i)->idtratto;

    const point_s * __point;

    for(count = 0; i<lenPoint; ++i){
        __point = page->at(i);
        if(__point->idtratto != idtratto
                || !__point->isIdUser())
            break;
        count ++;
    }
    return count;
}

static uint howReduce(uint &i,
                      const page *page,
                      const uint lenPoint){
    uint __m;
    __m = __howReduce(i, page, lenPoint);


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
static uint howMuchPoint(datastruct *data){
    uint i, pageCount, totalPointToRemove = 0;
    const uint lenPage = data->lengthPage();
    const page *page;

    for(pageCount = 0; pageCount < lenPage; pageCount ++){
        page = data->at(pageCount);
        const uint lenPoint = page->length();
        for(i = 0; i < lenPoint; ++i){
            totalPointToRemove += howReduce(i, page, lenPoint);
        }
    }

    return totalPointToRemove;
}
