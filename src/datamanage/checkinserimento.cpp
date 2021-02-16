#include "checkinserimento.h"

#include "iostream"
#include <QStringList>

bool checkinserimento(QList<QString> *lista, int max){
    int i = 0, lencompare;
    while(i < max){
        lencompare = lista->at(i+1).length() - lista->at(i).length();

        if(lencompare < 0)
            lencompare *= -1;

        if(lencompare <= 0 && lista->at(i).mid(0, lista->length() - lencompare) == lista->at(i+1)){

        }
        else if(lista->at(i+1).indexOf(lista->at(i)) == -1)
            return false;

        ++i;
    }
    return true;
}


