#include "memtest.h"
#include <QList>
#include <QVector>
#include "string.h"
#include "pthread.h"

#ifdef DEBUG_CORE

struct mem_info
{
    const void *pointer;
    char function[128];
    char file[128];
};

static pthread_mutex_t _mem_mutex;
static QList<mem_info> _mem;

void __init__ initMem(void)
{
    qDebug() << "Memtest enable";
    pthread_mutex_init(&_mem_mutex, NULL);
}

static void print_mem_info(const mem_info *mem)
{
    assert(mem);
    QString message;

    message = QString("Pointer %1 File: %2 Function %3").arg( QString::number((unsigned long)mem->pointer), mem->file, mem->function);

    qDebug() << message;
}

void WMalloc_private(const char *file, const char *function, const void *pointer)
{
    mem_info mem;

    strncpy(mem.file,       file,       sizeof(mem.file));
    strncpy(mem.function,   function,   sizeof(mem.function));

    mem.pointer = pointer;

    pthread_mutex_lock(&_mem_mutex);
    _mem.append(mem);
    pthread_mutex_unlock(&_mem_mutex);
}

void WFree_private(void *mem, const char *file, const char *function)
{
    int i;
    const mem_info *tmp;
    QList<mem_info> &__mem = _mem;
    int res = 0;
    QString msg;

    pthread_mutex_lock(&_mem_mutex);

    i = _mem.length();

    for(i --; i >= 0; i--){
        tmp = &__mem.at(i);
        if(tmp->pointer == mem){
            __mem.removeAt(i);
            res = 1;
            goto out;
        }
    }

out:
    pthread_mutex_unlock(&_mem_mutex);

    if(unlikely(!res)){
        msg = QString("Mem free not record. Pointer %1 File %2 Function %3").arg(QString::number((unsigned long)mem), file, function);
    }
}

void WEnd_application(void)
{
    if(likely(_mem.isEmpty())){
        qDebug() << "No mem leak";
        return;
    }
    qDebug() << "Mem leak";

    for(const auto &tmp : qAsConst(_mem)){
        print_mem_info(&tmp);
    }
}

#endif
