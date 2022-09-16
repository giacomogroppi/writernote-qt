#pragma once


#include <QString>

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

QString get_path_no_controll();
QString get_path(path::e_path);

