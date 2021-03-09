#ifndef GET_PATH_H
#define GET_PATH_H

namespace path {
    enum e_path: unsigned int{
        home,
        nameuser,
        audio_pos /* this return the folder to save the temporary audio */
    };
}

const char * get_path(path::e_path);

#endif // GET_PATH_H
