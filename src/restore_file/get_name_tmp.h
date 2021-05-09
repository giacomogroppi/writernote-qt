#ifndef GET_NAME_TMP_H
#define GET_NAME_TMP_H

#include <QString>
#include <QStringList>
#include "../utils/slash/slash.h"

class get_name_tmp
{
public:
    inline static QString get(QString *m_path){
        QStringList list = m_path->split(slash::__slash());
        int i;
        QString path = "";

        for(i = 0; i<list.length()-1; ++i){
            path += list.at(i);
        }

        path = "." + list.last() + "_tmp" ;

        return path;
    }
};

#endif // GET_NAME_TMP_H
