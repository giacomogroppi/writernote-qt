#include "datastruct.h"

void datastruct::removePointId(const QList<int> &list){
    uint i ;
    const uint len_list = list.length();
    for(i=0; i<len_list; i++){
        removePointId(list.at(i));
    }
}

void datastruct::removePointId(int id){
    uint i;

    for(page &page : this->m_page){
        uint lenPoint = page.length();

        for(i = 0; i < lenPoint;){
            if(page.at(i)->idtratto == id){
                page.removeAt(i);

                --lenPoint;
            }
            else{
                ++i;
            }
        }
    }
}
