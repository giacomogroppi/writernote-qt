#include "datastruct.h"
#include "utils/common_script.h"
#include "log/log_ui/log_ui.h"

void datastruct::removePointId(const QList<int> &listIndex, QList<int> *page){
    uint i;
    const uint len_list = listIndex.length();
    int __page = -1;

    if(page)
        page->clear();

    for(i = 0; i < len_list; i++){
        __page = removePointId(listIndex.at(i));

        if(page && IS_NOT_PRESENT_IN_LIST((*page), __page)){
            if(__page < 0){
                qWarning() << "No stroke in list with index" << listIndex.at(i);
                continue;
            }

            page->append(__page);
        }
    }
}

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

int datastruct::removePointId(const int id){
    int counterPage, counterStroke;
    const page *page;
    counterPage = this->lengthPage() - 1;

    for(; counterPage >= 0; counterPage --){
        page = &at(counterPage);

        counterStroke = page->lengthStroke() - 1;

        for(; counterStroke >= 0; counterStroke --){
            if(page->atStroke(counterStroke).getId() == id){
                at_mod(counterPage).removeAt(counterStroke);
                return counterPage;
            }
        }
    }

    return -1;
}
