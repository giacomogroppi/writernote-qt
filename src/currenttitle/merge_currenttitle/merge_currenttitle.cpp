#include "merge_currenttitle.h"
#include "currenttitle/document.h"

merge_currenttitle::n_merge merge_currenttitle::controll(
        Document *first,
        Document *second)
{
    if(!first->isRecorded() and !second->isRecorded())
        return audio_same;

    return n_merge::ok;
}

merge_currenttitle::n_merge merge_currenttitle::merge(Document *first, Document *second, n_audio audio)
{
    Document current_first(*first);
    Document current_second(*second);

    if(controll(first, second) != n_merge::ok)
        return n_merge::internal_error;

    W_ASSERT_TEXT(0, "NOT IMPLEMENTED");
    return merge_currenttitle::n_merge::internal_error;
}
