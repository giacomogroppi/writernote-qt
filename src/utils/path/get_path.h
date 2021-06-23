#ifndef GET_PATH_H
#define GET_PATH_H

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

const char *get_path_no_controll();
const char * get_path(path::e_path);

#endif // GET_PATH_H
