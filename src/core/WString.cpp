//
// Created by Giacomo Groppi on 30/04/23.
//

#include "WString.h"

#ifdef USE_QT
WString::WString(const QString &other)
    : _data(other.toUtf8().constData(), other.size())
{
}
#endif
