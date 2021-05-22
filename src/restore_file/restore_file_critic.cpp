#include "restore_file_critic.h"
#include <QFile>
#include "get_name_tmp.h"
#include "../utils/dialog_critic/dialog_critic.h"
#include "../dataread/xmlstruct.h"
#include "../datawrite/savefile.h"

restore_file_critic::n_err restore_file_critic::restore_file(const QString &path,
                                                             const QString &name_copybook)
{
    QString pos = get_name_tmp::get(path);

    currenttitle_class cur;
    indice_class ind;

    xmlstruct xml(pos, ind, cur);
    savefile save(path, cur);


    if(!xml.loadindice()){
        return restore_file_critic::n_err::error_load_index;
    }

    if(xml.loadfile(name_copybook+".xml") != OK){
        return restore_file_critic::n_err::error_load_file;
    }

    if(save.savefile_check_file() != OK){
        return restore_file_critic::n_err::error_save_file;
    }

    return n_err::restore_ok;
}
