#include "redoundo.h"

#include "../currenttitle/checksimilecopybook.h"

redoundo::redoundo(Document **data)
{
    this->m_current = data;

}

void redoundo::redo(Document **data){
    return;
    if(indice == 10){
        return;
    }

    *data = m_list.m_list[indice+1];

    indice ++;
}


/* reset the currenttitle */
void redoundo::undo(Document **data){
    return;
    if(indice == 0)
        return;

    *data = m_list.m_list[indice-1];

    indice --;
}


