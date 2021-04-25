#include "datastruct.h"

/*
 * the function return true if we need to create a new sheet
 */

bool datastruct::needtocreatenew(){

    uint len = posizionefoglio.length();

    if(isempty()
            || posizionefoglio.isEmpty()
            || len == 1)
        return true;

    double maxy;

    maxy = this->biggerynoid();

    return maxy > posizionefoglio.at(len-2);
}
