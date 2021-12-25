#ifndef LISTTHREADSAVE_H
#define LISTTHREADSAVE_H

#include <QList>
#include <QMutex>

// this function only manage
// thread save when appending
template <typename T>
class ListThreadSave: public QList<T>
{
private:
    QMutex mm;
public:
    ListThreadSave() = default;
    ~ListThreadSave() = default;

    Q_ALWAYS_INLINE void forceLock() { mm.lock(); };
    Q_ALWAYS_INLINE void unlock()    { mm.unlock(); };

    void from(const QList<T> &list){
        this = list;
    };
};

#endif // LISTTHREADSAVE_H
