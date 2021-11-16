#include "datastruct.h"

bool datastruct::userWrittenSomething(uint frompage)
{
    const uint len = lengthPage();
    for(; frompage<len; frompage++){
        if(at(frompage)->userWrittenSomething())
            return true;
    }

    return false;
}

/*
 * return true if the user has written something
*/
bool datastruct::userWrittenSomething(datastruct *s_data)
{
    uint l_first_page, l_sec_page, counterPage, counterPoint;
    const point_s * __f_p, *__s_p;
    bool check = false;
    const page *page1;
    const page *page2;

    l_first_page = lengthPage();
    if(s_data)
        l_sec_page = s_data->lengthPage();

    if(!s_data){
        counterPage = 0;
        goto single;
    }


    for(counterPage = 0; counterPage < l_first_page && counterPage < l_sec_page; counterPage++){

        page1 = this->at(counterPage);
        page2 = s_data->at(counterPage);

        const uint lenPoint1 = page1->length();
        const uint lenPoint2 = page2->length();

        for(counterPoint = 0; counterPoint < lenPoint1 && counterPoint < lenPoint2; counterPoint ++){
            __s_p = page2->at(counterPoint);
            __f_p = page1->at(counterPoint);

            if(memcmp(__s_p, __f_p, sizeof(*__f_p)) != 0){
                check = true;
                goto ret;
            }
        }
        if(lenPoint1 < lenPoint2){
            check = false;
            goto ret;
        }
    }

    if(l_first_page == l_sec_page){
        check = false;
        goto ret;
    }

    if((counterPage < l_sec_page) && s_data){
        check = s_data->userWrittenSomething(counterPage);
    }

    single:
    if(counterPage < l_first_page){
        check = userWrittenSomething(counterPage);
    }

    ret:
    return check;
}
