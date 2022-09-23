#pragma once

#include "utils/common_script.h"
#include "core/WMutex.h"
#include <QList>

// this function only manage
// thread save when appending
template <typename T>
class WListThreadSafe: public QList<T>, private WMutex{
private:
public:
    WListThreadSafe() = default;
    WListThreadSafe(const QList<T> &list);
    ~WListThreadSafe() = default;

    const T &at(qsizetype i) override
    {
        WMutex::lock();
        const auto &res = QList<T>::at(i);
        WMutex::unlock();
        return res;
    };
};

template<typename T>
WListThreadSafe<T>::WListThreadSafe(const QList<T> &list)
{
    this->m_list = list;
}
