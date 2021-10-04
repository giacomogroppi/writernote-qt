#include "redoundo.h"
#include "../currenttitle/checksimilecopybook.h"

void redoundo::copy(){

    if(indice < max){
        this->indice ++;

        this->m_list.operator[](indice) = **this->m_current;

        return;
    }
    if(indice == max){
        const auto &doc = this->m_list.takeFirst();
        append(doc);
        this->m_list.operator[](indice) = **this->m_current;
    }
}
