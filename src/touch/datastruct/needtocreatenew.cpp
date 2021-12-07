#include "datastruct.h"

/*
 * the function return true if we need to create a new sheet
 */

bool datastruct::needToCreateNewSheet() const{
    const uint lenPage = lengthPage();
    uint i, counterPage;
    const page *page;
    uint len;

    if(lenPage < 2)
        return true;

    for(counterPage = (lenPage - 2); counterPage < lenPage; counterPage ++){
        page = &at(counterPage);
        len = page->lengthStroke();
        for(i=0; i<len; i++){
            if(page->atStroke(i).length())
                    return true;
        }
    }
    return false;
}
