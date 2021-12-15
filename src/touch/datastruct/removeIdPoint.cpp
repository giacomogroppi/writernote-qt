#include "datastruct.h"
#include "../../utils/common_script.h"

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
