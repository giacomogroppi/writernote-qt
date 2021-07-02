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

    Document *temp = new Document;
    m_list[len] = temp;

}
