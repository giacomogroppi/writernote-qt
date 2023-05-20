#pragma once

#include "core/WString.h"
#include "core/WByteArray.h"
#include <functional>
#include <iostream>


#define VAL_RES "_res"
#define NAME_EXT "writernote"

namespace restore_file_critic
{
    enum n_err: int{
        error_load_file,
        error_save_file,
        restore_ok
    };

    enum n_err restore_file_direct(const WByteArray & path_load,
                                   const WByteArray& path_save,
                                   std::function<void(const WString &)> showMessage);

};
