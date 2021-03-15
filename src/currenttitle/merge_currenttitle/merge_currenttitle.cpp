#include "merge_currenttitle.h"


merge_currenttitle::n_merge merge_currenttitle::controll(
        currenttitle_class *first,
        currenttitle_class *second)
{
    if(!first->audio_position_path.isEmpty() && !second->audio_position_path.isEmpty())
        return audio_same;

    if(first->m_touch + second->m_touch == 1)
        return n_merge::not_compatible;

    return n_merge::ok;
}

merge_currenttitle::n_merge merge_currenttitle::merge(currenttitle_class *first, currenttitle_class *second)
{

}
