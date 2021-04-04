#ifndef REDOUNDO_H
#define REDOUNDO_H

#include "lista_copybook.h"

class redoundo
{
private:
    lista_copybook m_list;

    /* 0 <= indice <= 10 */
    unsigned short int indice = 0;

    currenttitle_class **m_current;

public:
    redoundo(currenttitle_class **);

    ~redoundo(){
        m_list.clear();
    };

    void copy(currenttitle_class *);

    void undo(currenttitle_class **);
    void redo(currenttitle_class **);

    void copy_b(currenttitle_class *, currenttitle_class *);

    void setting_data(currenttitle_class **data){
        m_current = data;

        indice = 0;
    }

};

#endif // REDOUNDO_H
