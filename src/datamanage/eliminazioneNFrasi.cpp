#include "../mainwindow.h"
#include "ui_mainwindow.h"

/*
funzione che permette di eliminare più grasi registrate
nello stesso secondo, in modo da occupare meno spazio
nel file finale *.writer

viene considerato il fatto che le strighe siano state
tutte corrette, in quanto gli altri step sono già stati eseguiti
precedentemente
*/
void eliminazioneNFrasi(MainWindow *parent){
    if(parent->m_currenttitle->posizione_iniz.length() != parent->m_currenttitle->testinohtml.length())
        return;
    int i = 1, lunghezza = parent->m_currenttitle->posizione_iniz.length();

    while(i < lunghezza){
        if(parent->m_currenttitle->posizione_iniz[i] == parent->m_currenttitle->posizione_iniz[i-1]){
            delete &parent->m_currenttitle->posizione_iniz[i-1];
            delete &parent->m_currenttitle->testinohtml[i-1];

            lunghezza -= 1;
        }
        else
            i++;
    }

}

