#ifndef RESTORE_FILE_CRITIC_H
#define RESTORE_FILE_CRITIC_H

#include <QString>
#include <QStringList>

#define VAL_RES "_res"
#define NAME_EXT "writernote"

class restore_file_critic
{
public:
    enum n_err: int{
        error_load_index,
        error_load_file,
        error_save_file,
        restore_ok
    };


    static enum n_err
            restore_file(const QString & path,
                         const QString & name_copybook);

};

#endif // RESTORE_FILE_CRITIC_H
