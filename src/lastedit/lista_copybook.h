#ifndef LISTA_COPYBOOK_H
#define LISTA_COPYBOOK_H

#define QUANTIINDICI 10
#include "../currenttitle/document.h"

class lista_copybook{
public:
    lista_copybook (){
        for(int i=0; i<QUANTIINDICI; i++){
            m_list[i] = NULL;
        }
    }

    void append();

    inline void clear(){
        for(int i=0; i<QUANTIINDICI && m_list[i]; i++){
            if(m_list[i]){
                delete m_list[i];
                m_list[i] = nullptr;
            }
        }
    }

    int lenght();

    Document *m_list[QUANTIINDICI];

};

#endif // LISTA_COPYBOOK_H
