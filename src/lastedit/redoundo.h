#ifndef REDOUNDO_H
#define REDOUNDO_H

#include "../currenttitle/currenttitle_class.h"
#define QUANTIINDICI 10



class redoundo
{
private:
    QList<currenttitle_class *> m_lista;

    /* 0 <= indice <= 10 */
    unsigned short int indice = 0;

    currenttitle_class *m_current;

    void scala(short numero);

    void append();
public:
    redoundo(currenttitle_class *);

    ~redoundo(){
        int i;
        for(i=0; i<QUANTIINDICI; i++){
            delete m_lista[i];
        }
    };

    void copy();

    void undo(currenttitle_class **);
    void redo(currenttitle_class **);

    void copy_b(currenttitle_class *, currenttitle_class *);

    void setting_data(currenttitle_class *data){
        int i;
        for(i=0; i<QUANTIINDICI; i++){
            m_lista[i]->reset();
        }
        m_current = data;

        indice = 0;
    }

};

#endif // REDOUNDO_H
