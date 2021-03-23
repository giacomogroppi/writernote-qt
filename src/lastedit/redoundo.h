#ifndef REDOUNDO_H
#define REDOUNDO_H

#include "../currenttitle/currenttitle_class.h"
#define QUANTIINDICI 10

class lista_copybook{
public:
    lista_copybook (){
        for(int i=0; i<QUANTIINDICI; i++){
            m_list[i] = NULL;
        }
    }

    void append();

    inline void clear(){
        for(int i=0; i<QUANTIINDICI; i++){
            if(m_list[i])
                delete m_list[i];
        }
    }

    int lenght();

    currenttitle_class *m_list[QUANTIINDICI];

};

void lista_copybook::append(){
    auto len = this->lenght();

    auto temp = new currenttitle_class;
    m_list[len] = temp;
}

int lista_copybook::lenght(){
    int i;
    for(i=0; m_list[i] != nullptr; i++);
    return i;
}

class redoundo
{
private:
    lista_copybook m_list;

    /* 0 <= indice <= 10 */
    unsigned short int indice = 0;

    currenttitle_class *m_current;

    void append();
public:
    redoundo(currenttitle_class *);

    ~redoundo(){
        int i;
        for(i=0; i<QUANTIINDICI; i++){
            delete m_list.m_list[i];
        }
    };

    void copy(currenttitle_class *);

    void undo(currenttitle_class **);
    void redo(currenttitle_class **);

    void copy_b(currenttitle_class *, currenttitle_class *);

    void setting_data(currenttitle_class *data){
        m_current = data;

        indice = 0;
    }

};

#endif // REDOUNDO_H
