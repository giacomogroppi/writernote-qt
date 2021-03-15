#ifndef MERGE_CURRENTTITLE_H
#define MERGE_CURRENTTITLE_H

#include "../currenttitle_class.h"

class merge_currenttitle
{
public:
    enum n_merge: int{
        audio_same,
        not_compatible, /* we cannot merge two files which are created in different ways [with the keyboard or pen] */
        ok
    };

    static n_merge controll(currenttitle_class *, currenttitle_class *);

    static n_merge merge(currenttitle_class *first, currenttitle_class *second);
};

#endif // MERGE_CURRENTTITLE_H
