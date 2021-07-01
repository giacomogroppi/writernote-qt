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
    const point_s * __f_p, *__s_p;
    bool check;

    l_first = length();
    if(s_data)
        l_sec = s_data->length();

    scala_all();

    if(s_data){
        s_data->scala_all();
    }else{
        i=0;
        goto single;
    }


    for(i=0; i<l_first && i<l_sec; ++i){
        __s_p = s_data->at(i);
        __f_p = at(i);

        if(memcmp(__s_p, __f_p, sizeof(*__f_p)) != 0){
            check = true;
            goto ret_;
        }
    }

    if(l_first == l_sec){
        check = false;
        goto ret_;
    }

    single:
    if(i < l_first){
        check = userWrittenSomething(i);
    }

    if(i < l_sec && s_data){
        check = s_data->userWrittenSomething(i);
    }



    ret_:
    restoreLastTranslation();

    if(s_data)
        s_data->restoreLastTranslation();
    return check;
}
