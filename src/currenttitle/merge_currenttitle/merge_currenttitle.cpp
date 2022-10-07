#include "merge_currenttitle.h"
#include "currenttitle/document.h"

merge_currenttitle::n_merge merge_currenttitle::controll(
        Document *first,
        Document *second)
{
    if(first->se_registato != Document::not_record && second->se_registato != Document::not_record)
        return audio_same;

    return n_merge::ok;
}

merge_currenttitle::n_merge merge_currenttitle::merge(Document *first, Document *second, n_audio audio)
{
    Document current_first;
    Document current_second;

    Document::copy(current_first, *first);
    Document::copy(current_second, *second);


    if(controll(first, second) != n_merge::ok)
        return n_merge::internal_error;

    int i, len;
    double last = first->biggery();
    Q_UNUSED(audio);
    Q_UNUSED(last);
    Q_UNUSED(i);
    Q_UNUSED(len);


    return n_merge::ok;
}
