#include "datastruct.h"

void datastruct::removePointId(QList<int> &list){
    unsigned int i, len, len_list;

    len_list = m_point.length();
    len = list.length();
    for(i=0; i<len; i++)
    {
        removePointId(list.at(i), len_list);
    }
}
void datastruct::removePointId(int id, int len){
    int i;
    if(len == -1)
        len = m_point.length();

    for(i=0; i<len; i++){
        if(m_point.at(i).idtratto == id){
            m_point.removeAt(i);
            --len;
        }
    }

}
