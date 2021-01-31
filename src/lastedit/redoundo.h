#ifndef REDOUNDO_H
#define REDOUNDO_H

#include "../currenttitle/currenttitle_class.h"


class redoundo
{
private:
    currenttitle_class last;
    currenttitle_class next;

    currenttitle_class *current;
public:
    redoundo(currenttitle_class *);

    void copy();

    void undo();
    void redo();

    bool check = true;
};

#endif // REDOUNDO_H
