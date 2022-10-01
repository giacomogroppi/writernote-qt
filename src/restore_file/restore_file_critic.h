#pragma once

#include <QString>
#include <QByteArray>

#define VAL_RES "_res"
#define NAME_EXT "writernote"

class restore_file_critic
{
public:
    enum n_err: int{
        error_load_file,
        error_save_file,
        restore_ok
    };


    static enum n_err
            restore_file(const QByteArray & path_save);

    static enum n_err
            restore_file_direct(const QByteArray & path_load,
                                const QByteArray& path_save);

};
