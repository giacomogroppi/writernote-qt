#include "datastruct.h"
#include "../../log/log_ui/log_ui.h"

/*
 * this function change the id of all point
 * with the same id of m_point[i]
*/
void datastruct::changeId(uint index, uint indexPage, uint lenPage, int base)
{
    int tmp;
    page *page;
    uint len, counterPage;
    if(base == -1)
        base = maxId() + 1;

    if(lenPage == 0){
        lenPage = this->lengthPage();
        if(!lenPage){
            LOG("datastruct::changeId lenPage = 0", log_ui::type_write::possible_bug);
        }
    }

    tmp = at(indexPage)->at(index)->idtratto;

    for(counterPage = 0; counterPage < lenPage; counterPage++){
        len = at(counterPage)->length();
        page = at_mod(counterPage);
        for(; index<len && page->at(index)->idtratto == tmp;
            index++)
            page->at_mod(index)->idtratto = base;
        index = 0; /* we need to reset the index */
    }
}
