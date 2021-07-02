#ifndef REDOUNDO_H
#define REDOUNDO_H

#include "lista_copybook.h"

class redoundo
{
private:
    lista_copybook m_list;

    /* 0 <= indice <= 10 */
    unsigned short int indice = 0;

    Document **m_current;

public:
    redoundo(Document **);

    ~redoundo(){
        m_list.clear();
    };

    void copy(Document *);

    void undo(Document **);
    void redo(Document **);

    void copy_b(Document *, const Document *);

    void setting_data(Document **data){
        m_current = data;

        indice = 0;
    }

};

#endif // REDOUNDO_H
