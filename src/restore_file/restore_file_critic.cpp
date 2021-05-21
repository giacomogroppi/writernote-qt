#include "restore_file_critic.h"
#include <QFile>
#include "get_name_tmp.h"
#include "../utils/dialog_critic/dialog_critic.h"
#include "../dataread/xmlstruct.h"
#include "../datawrite/savefile.h"

restore_file_critic::n_err restore_file_critic::restore_file(const QString &path,
                                                             const QString &name_copybook,
                                                             const bool all)
{
    QString pos = get_name_tmp::get(path);

    uint i, len;

    currenttitle_class cur;
    indice_class ind;

    xmlstruct xml(pos, ind, cur);
    savefile save(pos, cur);


    if(!xml.loadindice()){
        return restore_file_critic::n_err::error_load_index;
    }

    len = ind.titolo.length();

    if(all){
        for(i=0; i<len; ++i){
            if(xml.loadfile(ind.titolo.at(i)) != OK){
                continue;
            }

            save.setData(nullptr, &cur);
            if(save.savefile_check_file() != OK){
                return restore_file_critic::n_err::error_save_file;
            }

        }
    }else{
        if(xml.loadfile(name_copybook) != OK){
            return restore_file_critic::n_err::error_load_file;
        }

        if(save.savefile_check_file() != OK){
            return restore_file_critic::n_err::error_save_file;
        }
    }

    return n_err::restore_ok;
}
