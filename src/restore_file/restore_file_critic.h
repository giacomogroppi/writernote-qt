#pragma once

#include <QString>
#include <QByteArray>

#define VAL_RES "_res"
#define NAME_EXT "writernote"

namespace restore_file_critic
{
    enum n_err: int{
        error_load_file,
        error_save_file,
        restore_ok
    };

    enum n_err restore_file_direct( const QByteArray & path_load,
                                    const QByteArray& path_save);

};
