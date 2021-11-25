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
    uint counterPage;

    if(data->isempty())
        return 0;

    size = 0;
    for(counterPage = 0; counterPage < data->lengthPage(); counterPage ++){
        const page &page = data->at(counterPage);
        for(uint counterStroke = 0; counterStroke < page.lengthStroke(); counterStroke ++){
            size += page.atStroke(counterStroke).getSize();
        }
    }

    size += sizeof(double);

    return size;
}

void reduce_size::decrese(datastruct *data){
    uint i, pageCounter, len;
    const uint lenPage = data->lengthPage();

    if(!lenPage){
        return;
    }
    for(pageCounter=0; pageCounter<lenPage; pageCounter++){
        page &page = data->at_mod(pageCounter);

        len = page.lengthStroke();
        for(i=0; i<len; ++i){
            const stroke &stroke = page.atStroke(i);

            if(!stroke.isIdUser()){
                /* WE CAN'T REMOVE WRITERNOTE POINT */
                continue;
            }

            page.atStrokeMod(i).decreasePrecision();

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
