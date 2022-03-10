#include "redoundo.h"
#include "touch/tabletcanvas.h"
#include "testing/memtest.h"

void redoundo::append(Document *doc)
{
    this->m_list.append(doc);
    W_ASSERT(m_list.length() == max);
}

redoundo::redoundo(TabletCanvas *__canvas)
{
    W_ASSERT(__canvas);
    uint i;
    Document *doc;

    this->canvas = __canvas;

    for(i = 0; i < max; i++){
        WNew(doc, Document, ());
        m_list.append(doc);
    }
}

redoundo::~redoundo()
{
    int i;
    const int len = m_list.length();

    for(i = 0; i < len; ++i){
        WDelete(m_list.operator[](i));
    }
}

void redoundo::clear()
{
    uint i;
    const uint len = m_list.length();

    for(i=0; i<len; ++i){
        this->m_list.operator[](i)->reset();
    }
}

void redoundo::redo()
{
    if(indice == this->max - 1){
        return;
    }

    Document::copy(*this->m_list.operator[](indice+1), *canvas->data);

    indice ++;
}

/* go back to the last Document */
void redoundo::undo()
{
    if(indice == 0)
        return;

    Document::copy(*this->m_list.operator[](indice-1), *canvas->data);

    indice --;
}

void redoundo::copy()
{
    if(indice < max){
        Document::copy(*canvas->data, *m_list.operator[](indice));
        this->indice ++;
        return;
    }

    Document *point = this->m_list.takeFirst();
    append(point);
    Document::copy(*canvas->data, *m_list.operator[](indice-1));
}
