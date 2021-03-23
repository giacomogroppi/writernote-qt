#include "redoundo.h"

#include "../currenttitle/checksimilecopybook.h"

redoundo::redoundo(currenttitle_class *data)
{
    this->m_current = data;

}

void redoundo::redo( currenttitle_class **data){
    if(indice == 10){
        return;
    }

    *data = m_list.m_list[indice+1];

    indice ++;
}


/* reset the currenttitle */
void redoundo::undo(currenttitle_class **data){
    if(indice == 0)
        return;

    *data = m_list.m_list[indice-1];

    indice --;
}


