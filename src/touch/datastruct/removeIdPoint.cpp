#include "datastruct.h"

void datastruct::removePointId(QList<int> &list){
    unsigned int i, len, len_list;

    len_list = m_point.length();
    len = list.length();
    for(i=0; i<len; i++)
    {
        removePointId(list.at(i), &len_list);
    }
}

void datastruct::removePointId(int id,
                               unsigned int *len){
    unsigned int i;
    bool need_delete = false;
    if(!len){
        need_delete = true;
        len = (unsigned int *)malloc(sizeof (unsigned int));
    }

    if(need_delete)
        *len = m_point.length();

    for(i=0; i<*len;){
        if(m_point.at(i).idtratto == id){
            m_point.removeAt(i);
            --(*len);
        }
        else
            ++i;
    }

    if(need_delete)
        free(len);

}
