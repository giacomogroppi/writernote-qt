#include "redoundo.h"

void redoundo::append(Document *doc){
    this->m_list.append(doc);
    assert(m_list.length() == 10);
}

redoundo::redoundo(Document **data)
{
    uint i;
    Document *doc;

    this->m_current = data;

    for(i=0; i<max; i++){
        doc = new Document;
        m_list.append(doc);
    }
}

redoundo::~redoundo()
{
    uint i;
    const uint len = m_list.length();
    for(i=0; i<len; ++i){
        delete this->m_list.operator[](i);
    }
}

void redoundo::redo(){
    if(indice == this->max - 1){
        return;
    }

    Document::copy(*this->m_list.operator[](indice+1), **this->m_current);

    indice ++;
}

void redoundo::clear(){
    uint i;
    const uint len = m_list.length();
    for(i=0; i<len; ++i){
        this->m_list.operator[](i)->reset();
    }
}


/* go back to the last Document */
void redoundo::undo(){
    if(indice == 0)
        return;

    Document::copy(*this->m_list.operator[](indice-1), **this->m_current);

    indice --;
}

void redoundo::copy(){
    if(indice < max){
        Document::copy(**this->m_current, *m_list.operator[](indice));
        this->indice ++;
        return;
    }
    Document *point = this->m_list.takeFirst();
    append(point);
    Document::copy(**this->m_current, *m_list.operator[](indice-1));
}
