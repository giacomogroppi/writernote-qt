#pragma once

#include "core/WListFast.h"
#include "core/WVector.h"
#include "currenttitle/document.h"

class redoundo
{
private:

    /* 0 <= indice <= 10 */
    unsigned char indice = 0;
    WVector<Document *> m_list;
    class TabletCanvas *canvas;
    static constexpr int max = 10;
    Fn<Document*()> _getDoc;
    void append(Document *doc);

public:
    redoundo(TabletCanvas *m_canvas, Fn<Document*()> getDoc);
    ~redoundo();

    void copy();

    void undo();
    void redo();

    void clear();
};

