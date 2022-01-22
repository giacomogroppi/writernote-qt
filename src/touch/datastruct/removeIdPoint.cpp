#include "datastruct.h"
#include "utils/common_script.h"
#include "log/log_ui/log_ui.h"

void datastruct::removePointIndex(
        QVector<int>    &pos,
        cint            __page,
        cbool           __isOrder)
{
    page *page = &at_mod(__page);
    int i = pos.length() - 1;

    if(likely(__isOrder)){
#ifdef DEBUGINFO
        if(!is_order(pos)){
            std::abort();
        }
#else
        if(unlikely(is_order(pos))){
            NAME_LOG_EXT->write("List not order", log_ui::critic_error);
            /* in this case we need to order */
            order(pos);
        }

#endif
    }else{
        order(pos);
    }

    for(; i >= 0; i --){
        page->removeAt(pos.at(i));
    }
}

void datastruct::removePointIndex(
        QList<QVector<int> >    &pos,
        cint                    base,
        cbool                   __isOrder)
{
    int i, lenList;
    lenList = pos.length();

    for(i = 0; i < lenList; i++){
        removePointIndex(pos.operator[](i), base + i, __isOrder);
    }

}

