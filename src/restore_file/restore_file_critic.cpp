#include "restore_file_critic.h"
#include <QFile>
#include "../utils/dialog_critic/dialog_critic.h"

restore_file_critic::restore_file_critic()
{

}

restore_file_critic::n_err restore_file_critic::restore_file(QString &path)
{
    QString pos = add_prefix(path);
    if(!QFile::exists(pos)){
        messaggio_utente("There is in pos " + pos);
    }
    else{

    }

}
