#include "datastruct.h"

void datastruct::removePointId(const QList<int> &listIndex, QList<int> *page){
    uint i;
    const uint len_list = listIndex.length();
    int __page = -1;

    if(page)
        page->clear();

    for(i=0; i<len_list; i++){
        __page = removePointId(listIndex.at(i));

        if(page && page->indexOf(__page) == -1){
            if(__page < 0){
                qWarning() << "No stroke in list with index" << listIndex.at(i);
                continue;
            }

            page->append(__page);
        }
    }
}

int datastruct::removePointId(const int id){
    uint counterPage, counterStroke;
    const page *page;
    const uint lenPage = this->lengthPage();

    for(counterPage = 0; counterPage < lenPage; counterPage ++){
        page = &at(counterPage);

        const uint lenStroke = page->lengthStroke();

        for(counterStroke = 0; counterStroke < lenStroke; counterStroke ++){
            if(page->atStroke(counterStroke).getId() == id){
                at_mod(counterPage).removeAt(counterStroke);
                return counterPage;
            }
        }
    }

    return -1;
}
