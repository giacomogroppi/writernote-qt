#pragma once

#include <QString>
#include <QStringList>
#include "utils/slash/slash.h"

class get_name_tmp
{
public:
    inline static QString get(const QString &m_path){
        QStringList list;
        QString path;
        const char sl = slash::__slash();
        uchar last_pos=0;

        path = m_path;

        last_pos = path.lastIndexOf(sl);

        path.insert(last_pos+1, '.');

        path += "_tmp";

        return path;

    }
};

