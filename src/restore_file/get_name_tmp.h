#ifndef GET_NAME_TMP_H
#define GET_NAME_TMP_H

#include <QString>
#include <QStringList>
#include "../utils/slash/slash.h"

class get_name_tmp
{
public:
    inline static QString get(QString *m_path){
        QStringList list;
        int i;
        QString path;
        char sl;
        uchar last_pos;

        path = *m_path;
        sl = slash::__slash();

        for(i=0; i<m_path->length(); ++i){
            if(m_path->at(i) == sl){
                last_pos = i;
            }
        }

        path.insert(last_pos+1, '.');
        path += "_tmp";

        return path;

        /*list = m_path->split(sl);
        path = "";

        for(i = 0; i<list.length()-1; ++i){
            path += list.at(i);
        }

        path = "." + list.last() + "_tmp" ;

        return path;*/
    }
};

#endif // GET_NAME_TMP_H
