#include "redoundo.h"

/*
 * la funzione scala tutti gli indici in quanto
 * bisogna crearne uno nuovo da inserire in coda,
 * e così resetta tutti tutto il copybook
 *
 * in caso l'indice sia < 10
 * non dobbiamo fare niente in quanto base aggiungere alla fine il
 * dato [nella posizione indice] e poi aumentarlo
 *
 * in caso l'indice sia 10 [e non possiamo aggiungere più altri indici]
 * se numero è > 0 allora vuol dire che stiamo aggiungendo
 * in coda, quindi dobbiamo prendere il primo puntatore, salvarlo
 * in temp, e spostare tutti gli elementi di uno
 *
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


    if(this->indice < QUANTIINDICI){

        return;
    }else{

    }

}
