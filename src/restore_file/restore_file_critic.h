#ifndef RESTORE_FILE_CRITIC_H
#define RESTORE_FILE_CRITIC_H

#include <QString>
#include <QStringList>

#define VAL_RES "_res"
#define NAME_EXT "writernote"

class restore_file_critic
{
public:
    restore_file_critic();

    enum n_err: int{

    };

    static QString add_prefix(const QString &string){
        int index;
        QString res = string;


        index = res.indexOf(".writer");
        if(index != -1){
            res = res.mid(0, index) + VAL_RES + ".writer";
        }else{
            res = string + VAL_RES + "." + NAME_EXT;
        }

        return res;
    }

    static enum n_err
            restore_file(QString & path);

};

#endif // RESTORE_FILE_CRITIC_H
