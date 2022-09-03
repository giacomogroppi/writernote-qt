#pragma once

class merge_currenttitle
{
public:
    enum n_merge: int{
        audio_same,
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

    static n_merge controll(class Document *, Document *);

    static n_merge merge(Document *first, Document *second, n_audio);

};

