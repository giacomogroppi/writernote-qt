#include "reduce_size.h"
#include "touch/datastruct/datastruct.h"
#include "touch/datastruct/page.h"

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
    int counterPage;

    if(data->isempty())
        return 0;

    size = 0;
    for(counterPage = 0; counterPage < data->lengthPage(); counterPage ++){
        const page &page = data->at(counterPage);
        for(int counterStroke = 0; counterStroke < page.lengthStroke(); counterStroke ++){
            size += page.atStroke(counterStroke).getSizeInMemory();
        }
    }

    /* zoom */
    size += sizeof(double);

    return size;
}

void reduce_size::decrese(datastruct *data){
    int i, pageCounter, len;
    const int lenPage = data->lengthPage();

    if(!lenPage){
        return;
    }
    for(pageCounter=0; pageCounter<lenPage; pageCounter++){
        page &page = data->at_mod(pageCounter);

        len = page.lengthStroke();
        for(i=0; i<len; ++i){
            page.atStrokeMod(i).decreasePrecision();

        }
    }


}

/* the function calculates the points for each section */
static uint __howReduce(uint &i, const page *page){
    const stroke &stroke = page->atStroke(i);
    if(likely(stroke.is_normal()))
        return stroke.length();

    // we cannot decrease accuracy if the stroke is complex
    return 0;
}

static uint howReduce(uint &i,
                      const page *page){
    uint __m;
    __m = __howReduce(i, page);


    /*
     * we can't remove the first
     * and the end point
    */

    return (__m > 2) ? (__m - 2) : 0;
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
        page = &data->at(pageCount);

        const uint lenPoint = page->lengthStroke();
        for(i = 0; i < lenPoint; ++i){
            totalPointToRemove += howReduce(i, page);
        }
    }

    return totalPointToRemove;
}
