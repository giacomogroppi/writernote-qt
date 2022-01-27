#include "datastruct.h"

bool datastruct::userWrittenSomething(uint frompage)
{
    const uint len = lengthPage();
    for(; frompage < len; frompage++){
        if(at(frompage).userWrittenSomething())
            return true;
    }

    return false;
}

/*
 * return true if the user has written something
*/
bool datastruct::userWrittenSomething(datastruct *s_data)
{
    int l_first_page, l_sec_page, counterPage, counterStroke;
    bool check = false;

    l_first_page = lengthPage();
    if(s_data)
        l_sec_page = s_data->lengthPage();

    if(!s_data){
        counterPage = 0;
        goto single;
    }

    for(counterPage = 0; counterPage < l_first_page && counterPage < l_sec_page; counterPage++){

        const page &page1 = this->at(counterPage);
        const page &page2 = s_data->at(counterPage);

        for(counterStroke = 0; counterStroke < page1.lengthStroke() && counterStroke < page2.lengthStroke(); counterStroke ++){
            const stroke &firstStroke  = page2.atStroke(counterStroke);
            const stroke &secondStroke = page1.atStroke(counterStroke);

            for(int i = 0; i < firstStroke.length() && i < secondStroke.length(); i++){
                const point_s &firstPoint = firstStroke.at(i);
                const point_s &secondPoint = secondStroke.at(i);

                if(memcmp(&firstPoint, &secondPoint, sizeof(firstPoint)) != 0){
                    check = false;
                    goto ret;
                }
            }

            if(firstStroke.length() < secondStroke.length()){
                check = false;
                goto ret;
            }

        }

        if(page1.lengthStroke() < page2.lengthStroke()){
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
