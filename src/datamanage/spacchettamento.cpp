#ifndef SPACCHETTAMENTO_CPP
#define SPACCHETTAMENTO_CPP

#include "../mainwindow.h"
#include "checkinserimento.cpp"
#include "check1.cpp"

void spacchettamento(MainWindow *parent){
    int i, j, k, lunghezza = parent->self->currenttitle.testinohtml.length();
    for(i = 1; i < lunghezza; i++){
        while(!checkinserimento(&parent->self->currenttitle.testinohtml, i)){
            for(j=1; j <= i ; j++){
                if (parent->self->currenttitle.testinohtml[j].length() > parent->self->currenttitle.testinohtml[j-1].length()){

                    for(k = 0; k < parent->self->currenttitle.testinohtml[j].length() && k < parent->self->currenttitle.testinohtml[j-1].length(); k++)

                        if( (parent->self->currenttitle.testinohtml[j][k] != parent->self->currenttitle.testinohtml[j-1][k])
                                && k != parent->self->currenttitle.testinohtml[j-1].length()){

                            parent->self->currenttitle.testinohtml[j-1] = parent->self->currenttitle.testinohtml[j-1].mid(0, k) +
                                    parent->self->currenttitle.testinohtml[j][k] +
                                    parent->self->currenttitle.testinohtml[j-1].mid(k, -1);

                            break;
                        }

                }
                else if( parent->self->currenttitle.testinohtml[j].length() <= parent->self->currenttitle.testinohtml[j-1].length()){
                    for(k=0; k < parent->self->currenttitle.testinohtml[j-1].length() && parent->self->currenttitle.testinohtml[j].length(); k++)
                        if(parent->self->currenttitle.testinohtml[j][k] != parent->self->currenttitle.testinohtml[j-1][k]){
                            parent->self->currenttitle.testinohtml[j-1] = parent->self->currenttitle.testinohtml[j-1].mid(0, k)
                                    + parent->self->currenttitle.testinohtml[j-1].mid(k+1, -1);
                            break;
                        }

                }
            }
        }
    }

    int difference;

    qDebug() << "Passa all'altro ciclo";

    /* parte di funzione che aggiusta il testo in caso si fosse modificato in mezzo */
    while(!check1(&parent->self->currenttitle.testinohtml)){
        i = 1;

        lunghezza = parent->self->currenttitle.testinohtml.length();

        while(i < lunghezza){
            if(parent->self->currenttitle.testinohtml[i].length()
                    < parent->self->currenttitle.testinohtml[i-1].length()){

                difference = parent->self->currenttitle.testinohtml[i].length() - parent->self->currenttitle.testinohtml[i-1].length();
                parent->self->currenttitle.testinohtml[i-1] = parent->self->currenttitle.testinohtml[i-1].mid(0, difference);
            }

            if(parent->self->currenttitle.testinohtml[i].length() == parent->self->currenttitle.testinohtml[i-1].length()
                    || parent->self->currenttitle.testinohtml[i][parent->self->currenttitle.testinohtml[i].length() - 1] == ' '){
                parent->self->currenttitle.testinohtml.removeAt(i);
                parent->self->currenttitle.posizione_iniz.removeAt(i);
                -- lunghezza;
            }
            else
                ++i;
        }
    }
}

#endif //SPACCHETTAMENTO_CPP
