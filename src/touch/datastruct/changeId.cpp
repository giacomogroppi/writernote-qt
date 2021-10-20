#include "datastruct.h"

/*
 * this function change the id of all point
 * with the same id of m_point[i]
*/
void datastruct::changeId(uint index, uint indexPage, uint lenPage, int base)
{
    int tmp;
    page *page;
    uint len;
    if(base == -1)
        base = maxId() + 1;

    if(lenPage == 0){
        lenPage = lengthPoint();
        if(!lenPage)
            return;
    }

    tmp = at(index, indexPage)->idtratto;

    for(uint k = 0; k<lenPage; k++){
        len = at(k)->length();
        page = at_mod(k);
        for(; index<len && page->at(index)->idtratto == tmp;
            index++)
            page->at_mod(index)->idtratto = base;
        index = 0; /* we need to reset the index */
    }
}
