#include "lista_copybook.h"

int lista_copybook::lenght(){
    int i;
    for(i=0; m_list[i]; i++);
    return i;
}

void lista_copybook::append(){
    int len = lenght();
    if(len == QUANTIINDICI)
        return;

    currenttitle_class *temp = new currenttitle_class;
    m_list[len] = temp;

}
