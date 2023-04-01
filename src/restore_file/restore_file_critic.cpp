#include "restore_file_critic.h"
#include <QFile>
#include "dataread/xmlstruct.h"
#include "datawrite/savefile.h"

restore_file_critic::n_err restore_file_critic::restore_file_direct(
        const QByteArray &path_load,
        const QByteArray &path_save,
        std::function<void(const QString &)> showMessage)
{
    Document cur;

    xmlstruct xml(path_load, cur);
    savefile save(&path_save, &cur);
    const int res = xml.loadfile(false, false);

    if (!xmlstruct::manageMessage(res, showMessage)) {
        return restore_file_critic::n_err::error_load_file;
    }

    if (save.savefile_check_file(true) != OK) {
        return restore_file_critic::n_err::error_save_file;
    }

    return n_err::restore_ok;
}
