#include "redoundo.h"

#include "../currenttitle/checksimilecopybook.h"

redoundo::redoundo(currenttitle_class *data)
{
    this->m_current = data;

    currenttitle_class *temp;
    int i;

    for(i=0; i<QUANTIINDICI; i++){
        temp = new currenttitle_class;
        this->m_lista.append(temp);
    }
}

void redoundo::redo( currenttitle_class **data){
    if(indice == 10){
        return;
    }

    *data = m_lista.at(indice+1);

    indice ++;
}


/* reset the currenttitle */
void redoundo::undo(currenttitle_class **data){
    if(indice == 0)
        return;

    *data = m_lista.at(indice-1);

    indice --;
}


