#include "restore_file_critic.h"
#include "dataread/xmlstruct.h"
#include "datawrite/savefile.h"

restore_file_critic::n_err restore_file_critic::restore_file_direct(
        const WByteArray &path_load,
        const WByteArray &path_save,
        Fn<void(const WString &)> showMessage)
{
    Document curr;

    xmlstruct xml(path_load, curr);
    savefile save(&path_save, &curr);

    const int res = xml.loadfile(false, false);

    if (!xmlstruct::manageMessage(res, showMessage)) {
        return restore_file_critic::n_err::error_load_file;
    }

    if (save.savefile_check_file(true) != OK) {
        return restore_file_critic::n_err::error_save_file;
    }

    return n_err::restore_ok;
}
