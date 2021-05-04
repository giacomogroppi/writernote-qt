#include "datastruct.h"

/*
 * this function change the id of all point
 * with the same id of m_point[i]
*/
void datastruct::changeId(uint i, uint len, int base)
{
    if(base == -1)
        base = maxId() + 1;

    if(len == 0){
        len = length();
        if(!len)
            return;
    }

    int temp = at(i)->idtratto;

    for(; i<len && m_point.at(i).idtratto == temp;
        i++)
        at_mod(i)->idtratto = base;
}
