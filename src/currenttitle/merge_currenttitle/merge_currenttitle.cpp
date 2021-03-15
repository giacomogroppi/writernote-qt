#include "merge_currenttitle.h"


merge_currenttitle::n_merge merge_currenttitle::controll(
        currenttitle_class *first,
        currenttitle_class *second)
{
    if(first->se_registato != audio_record::not_record && second->se_registato != audio_record::not_record)
        return audio_same;

    if(first->m_touch + second->m_touch == 1)
        return n_merge::not_compatible;

    return n_merge::ok;
}

merge_currenttitle::n_merge merge_currenttitle::merge(currenttitle_class *first, currenttitle_class *second, n_audio audio)
{
    currenttitle_class current_first;
    currenttitle_class current_second;

    currenttitle_class::copy(&current_first, first);
    currenttitle_class::copy(&current_second, second);


    if(controll(first, second) != n_merge::ok)
        return n_merge::internal_error;

    int i, len;

    double last = first->datatouch->biggery();

    if(first->m_touch){


    }else{

    }


    return n_merge::ok;
}
