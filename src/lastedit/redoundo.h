#pragma once

#include <QList>
#include <QVector>
#include "currenttitle/document.h"

class redoundo
{
private:

    /* 0 <= indice <= 10 */
    uchar indice = 0;
    QVector<Document *> m_list;
    class TabletCanvas *canvas;
    static constexpr int max = 10;

    void append(Document *doc);
public:
    redoundo(TabletCanvas *m_canvas);
    ~redoundo();

    void copy();

    void undo();
    void redo();

    void clear();
};

