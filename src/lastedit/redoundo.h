#ifndef REDOUNDO_H
#define REDOUNDO_H
#include <QList>
#include "currenttitle/document.h"

class redoundo
{
private:

    /* 0 <= indice <= 10 */
    uchar indice = 0;
    QList<Document *> m_list;
    Document **m_current;
    const uint max = 10;

    void append(Document *doc);
public:
    redoundo(Document **);
    ~redoundo();

    void copy();

    void undo();
    void redo();

    void clear();
};

#endif // REDOUNDO_H
