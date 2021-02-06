#include "redoundo.h"

/*
 * la funzione scala tutti gli indici in quanto
 * bisogna crearne uno nuovo da inserire in coda,
 * e così resetta tutti tutto il copybook
*/

void redoundo::scala(short int numero){
    int i;

    currenttitle_class *temp;


    if(numero < 0){
        temp = m_lista.first();

        for(i=0; i<QUANTIINDICI-1; i++){
            m_lista[i] = m_lista[i+1];
        }

        m_lista[i+1] = temp;
        m_lista[i+1]->reset();

        return;
    }

    temp = m_lista.last();

    for(i=QUANTIINDICI-1; i>0; i--){
        m_lista[i] = m_lista[i-1];
    }

    m_lista[0] = temp;
    m_lista[0]->reset();

}
