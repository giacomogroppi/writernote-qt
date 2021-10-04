#ifndef REDOUNDO_H
#define REDOUNDO_H
#include <QList>
#include "../currenttitle/document.h"

class redoundo
{
private:

    /* 0 <= indice <= 10 */
    uchar indice = 0;
    QList<Document> m_list;
    Document **m_current;
    const uint max = 10;

    inline void append(const Document &doc){
        this->m_list.append(doc);
        assert(m_list.length() == 10);
    }
public:
    redoundo(Document **);

    ~redoundo(){
        m_list.clear();
    };

    void copy();

    void undo();
    void redo();

    void setting_data(Document **data){
        m_current = data;

        indice = 0;
    }

    inline void clear(){
        uint i;
        for(i=0; i<max; i++){
            m_list.operator[](i).reset();
        }
    }



};

#endif // REDOUNDO_H
