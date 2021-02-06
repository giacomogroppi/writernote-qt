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

    m_temp = new currenttitle_class;
}

void redoundo::redo(){
    if(indice == 10)
        return;

    copy_b(m_temp, m_current);

    copy_b(m_current, m_lista.at(indice+1));

    copy_b(m_lista.at(indice), m_temp);

    indice ++;
}


/* reset the currenttitle */
void redoundo::undo(){
    if(indice == 0)
        return;

    copy_b(m_temp, m_current);

    copy_b(m_current, m_lista.at(indice-1));

    copy_b(m_lista.at(indice), m_temp);

    indice --;
}


