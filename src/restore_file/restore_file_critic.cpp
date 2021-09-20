#include "restore_file_critic.h"
#include <QFile>
#include "get_name_tmp.h"
#include "../utils/dialog_critic/dialog_critic.h"
#include "../dataread/xmlstruct.h"
#include "../datawrite/savefile.h"

restore_file_critic::n_err restore_file_critic::restore_file(const QString &path)
{
    QString pos = get_name_tmp::get(path);

    return restore_file_direct(pos, path);
}

restore_file_critic::n_err restore_file_critic::restore_file_direct(const QString &path_load,
                                                                    const QString &path_save)
{
    Document cur;

    xmlstruct xml(path_load, cur);
    savefile save(path_save, cur);

    if(xml.loadfile(false, false) != OK){
        return restore_file_critic::n_err::error_load_file;
    }

    if(save.savefile_check_file() != OK){
        return restore_file_critic::n_err::error_save_file;
    }

    return n_err::restore_ok;
}
