#include "datastruct.h"

bool datastruct::userWrittenSomething(uint i)
{
    uint len;
    len = length();

    for(; i<len; ++i){
        if(datastruct::isIdUser(at(i)))
            return true;
    }

    return false;
}

/*
 * return true if the user has written something
*/
bool datastruct::userWrittenSomething(datastruct *s_data)
{
    uint l_first, l_sec, i;
    const point_s * __point, *__s_p;

    l_first = length();

    scala_all();
    s_data->scala_all();

    l_sec = s_data->length();

    for(i=0; i<l_first && i<l_sec; ++i){
        __s_p = s_data->at(i);
        __point = at(i);

        if(memcmp(__s_p, __point, sizeof(point_s)) != 0)
            goto ret_;
    }

    if(i < l_first){
        userWrittenSomething(i);
    }
    else{
        if(i < l_sec){
            s_data->userWrittenSomething(i);
        }
    }


    ret_:
    restoreLastTranslation();
    s_data->restoreLastTranslation();
    return true;
}
