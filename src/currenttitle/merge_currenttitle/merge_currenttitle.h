#ifndef MERGE_CURRENTTITLE_H
#define MERGE_CURRENTTITLE_H

#include "../document.h"

class merge_currenttitle
{
public:
    enum n_merge: int{
        audio_same,
        not_compatible, /* we cannot merge two files which are created in different ways [with the keyboard or pen] */
        ok,
        internal_error
    };

    /*
     * it is used to figure out if audio
     * needs to be copied, and if so,
     * which audio to copy
    */
    enum n_audio: int{
        no_copy,
        copy_first,
        copy_second
    };

    static n_merge controll(Document *, Document *);

    static n_merge merge(Document *first, Document *second, n_audio);

};

#endif // MERGE_CURRENTTITLE_H
