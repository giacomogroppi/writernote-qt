#pragma once


#include "core/String/WString.h"

namespace path {
    enum e_path: unsigned int{
        home,
        nameuser,
        audio_pos, /* this return the folder to save the temporary audio */
        log, /* default position log */
        tmp_file_not_save
    };
}

#define POS_TEMP_AUDIO "tmp_audio.wav"

WString get_path_no_controll();

