#pragma once

#include <QList>
#include <QMutex>

// this function only manage
// thread save when appending
template <typename T>
class ListThreadSave{
private:
    QMutex mm;

public:
    ListThreadSave() = default;
    ListThreadSave(const QList<T> &list);
    ~ListThreadSave() = default;

    Q_ALWAYS_INLINE void forceLock() { mm.lock(); };
    Q_ALWAYS_INLINE void unlock()    { mm.unlock(); };

    void from(const QList<T> &list){
        this->m_list = list;
    };

    QList<T> m_list;
};

template<typename T>
ListThreadSave<T>::ListThreadSave(const QList<T> &list)
{
    this->m_list = list;
}
