#ifndef GET_PATH_H
#define GET_PATH_H

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

#define POS_TEMP_AUDIO "_temp.wav"
#define POS_AUDIO(x) x->nome_copybook + POS_TEMP_AUDIO

QString get_path_no_controll();
QString get_path(path::e_path);

#endif // GET_PATH_H
