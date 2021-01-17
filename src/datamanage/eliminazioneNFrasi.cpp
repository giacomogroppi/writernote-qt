#include "../mainwindow.h"
#include "../self_class.h"
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
    if(parent->self->currenttitle.posizione_iniz.length() != parent->self->currenttitle.testinohtml.length())
        return;
    int i = 1, lunghezza = parent->self->currenttitle.posizione_iniz.length();

    while(i < lunghezza){
        if(parent->self->currenttitle.posizione_iniz[i] == parent->self->currenttitle.posizione_iniz[i-1]){
            delete &parent->self->currenttitle.posizione_iniz[i-1];
            delete &parent->self->currenttitle.testinohtml[i-1];

            lunghezza -= 1;
        }
        else
            i++;
    }

}

