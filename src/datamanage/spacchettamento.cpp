#include "../mainwindow.h"
#include "checkinserimento.h"
#include "check1.h"
#include "spacchettamento.h"

#define CURRENTTITLE m_parent->m_currenttitle

void spacchettamento::esecuzione(){
    int i, j, k, lunghezza = CURRENTTITLE.testinohtml.length();
    for(i = 1; i < lunghezza; i++){
        while(!checkinserimento(&CURRENTTITLE.testinohtml, i)){
            for(j=1; j <= i ; j++){
                if (CURRENTTITLE.testinohtml[j].length() > CURRENTTITLE.testinohtml[j-1].length()){

                    for(k = 0; k < CURRENTTITLE.testinohtml[j].length() && k < CURRENTTITLE.testinohtml[j-1].length(); k++)

                        if( (CURRENTTITLE.testinohtml[j][k] != CURRENTTITLE.testinohtml[j-1][k])
                                && k != CURRENTTITLE.testinohtml[j-1].length()){

                            CURRENTTITLE.testinohtml[j-1] = CURRENTTITLE.testinohtml[j-1].mid(0, k) +
                                    CURRENTTITLE.testinohtml[j][k] +
                                    CURRENTTITLE.testinohtml[j-1].mid(k, -1);

                            break;
                        }

                }
                else if(CURRENTTITLE.testinohtml[j].length() <= CURRENTTITLE.testinohtml[j-1].length()){
                    for(k=0; k < CURRENTTITLE.testinohtml[j-1].length() && CURRENTTITLE.testinohtml[j].length(); k++)
                        if(CURRENTTITLE.testinohtml[j][k] != CURRENTTITLE.testinohtml[j-1][k]){
                            CURRENTTITLE.testinohtml[j-1] = CURRENTTITLE.testinohtml[j-1].mid(0, k)
                                    + CURRENTTITLE.testinohtml[j-1].mid(k+1, -1);
                            break;
                        }

                }
            }
        }
        emit progress(i/lunghezza*100);
    }

    int difference;

    /* parte di funzione che aggiusta il testo in caso si fosse modificato in mezzo */
    while(!check1(&CURRENTTITLE.testinohtml)){
        i = 1;

        lunghezza = CURRENTTITLE.testinohtml.length();

        while(i < lunghezza){
            if(CURRENTTITLE.testinohtml[i].length()
                    < CURRENTTITLE.testinohtml[i-1].length()){

                difference = CURRENTTITLE.testinohtml[i].length() - CURRENTTITLE.testinohtml[i-1].length();
                CURRENTTITLE.testinohtml[i-1] = CURRENTTITLE.testinohtml[i-1].mid(0, difference);
            }

            if(CURRENTTITLE.testinohtml[i].length() == CURRENTTITLE.testinohtml[i-1].length()
                    || CURRENTTITLE.testinohtml[i][CURRENTTITLE.testinohtml[i].length() - 1] == ' '){
                CURRENTTITLE.testinohtml.removeAt(i);
                CURRENTTITLE.posizione_iniz.removeAt(i);
                -- lunghezza;
            }
            else
                ++i;
        }
    }

    emit finished();
}
