#include "redoundo.h"

redoundo::redoundo(Document **data)
{
    uint i;
    Document doc;
    this->m_current = data;

    for(i=0; i<max; i++){
        m_list.append(doc);
    }
}

void redoundo::redo(){
    return;
    if(indice == 10){
        return;
    }

    *this->m_current = &this->m_list.operator[](indice+1);

    indice ++;
}


/* reset the currenttitle */
void redoundo::undo(){
    return;
    if(indice == 0)
        return;

    *this->m_current = &this->m_list.operator[](indice-1);

    indice --;
}


