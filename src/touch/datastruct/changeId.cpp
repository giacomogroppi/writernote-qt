#include "datastruct.h"

/*
 * this function change the id of all point
 * with the same id of m_point[i]
*/
void datastruct::changeId(uint i, uint len, int base)
{
    int tmp;
    if(base == -1)
        base = maxId() + 1;

    if(len == 0){
        len = length();
        if(!len)
            return;
    }

    tmp = at(i)->idtratto;

    for(; i<len && at(i)->idtratto == tmp;
        i++)
        at_mod(i)->idtratto = base;
}
