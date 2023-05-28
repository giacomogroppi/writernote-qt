#include "redoundo.h"
#include "testing/memtest.h"

void redoundo::append(Document *doc)
{
    this->m_list.append(doc);
    W_ASSERT(m_list.size() == max);
}

redoundo::redoundo(TabletCanvas *__canvas, std::function<Document *()> getDoc)
    : _getDoc(getDoc)
{
    uint i;
    Document *doc;

    W_ASSERT(__canvas);

    this->canvas = __canvas;

    for(i = 0; i < max; i++){
        WNew(doc, Document, ());
        m_list.append(doc);
    }
}

redoundo::~redoundo()
{
    int i;

    for(i = 0; i < max; ++i){
        WDelete(m_list.operator[](i));
    }
}

void redoundo::clear()
{
    for (auto &l : this->m_list) {
        l->reset();
    }
}

void redoundo::redo()
{
    if (indice == this->max - 1) {
        return;
    }

    Document::copy(*this->m_list.operator[](indice + 1), *_getDoc());

    indice ++;
}

/* go back to the last Document */
void redoundo::undo()
{
    if(indice == 0)
        return;

    Document::copy(*this->m_list.operator[](indice-1), *_getDoc());

    indice --;
}

void redoundo::copy()
{
    if (indice < max) {
        Document::copy(*_getDoc(), *m_list.operator[](indice));
        this->indice ++;
        return;
    }

    Document *point = this->m_list.takeFirst();
    append(point);
    Document::copy(*_getDoc(), *m_list.operator[](indice-1));
}
