#include "memtest.h"
#include "core/WListFast.h"
#include "core/WVector.h"
#include "string.h"
#include "pthread.h"
#include "core/WMutexLocker.h"
#include "core/WMutex.h"

#if defined(DEBUG_MEM) && defined(USE_QT)
#include <QString>

struct mem_info
{
    const void *pointer;
    char function[128];
    char file[128];
    unsigned long int line;
};

static WMutex _mem_mutex;
static WListFast<mem_info> *_mem;

void __init__ initMem(void)
{
    qDebug() << "Memtest enable";
    _mem = new WListFast<mem_info>();
}

static void print_mem_info(const mem_info *mem)
{
    assert(mem);
    QString message;

    if(mem->line > 50000 || !mem->line){
        return;
    }

    message = QString("Pointer %1 File: %2 Function %3 Line %4").arg(QString::number((quint64)mem->pointer, 16),
                                                                         mem->file,
                                                                         mem->function,
                                                                         QString::number(mem->line));

    qDebug() << message;
}

void *WMalloc_private(const char *function, const char *file, unsigned long int line, const size_t size)
{
    mem_info mem;

    void *pointer = malloc(size);

    strncpy(mem.file,       file,       sizeof(mem.file));
    strncpy(mem.function,   function,   sizeof(mem.function));
    mem.line = line;

    mem.pointer = pointer;

    {
        WMutexLocker _(_mem_mutex);
        _mem->append(mem);
    }

    if(line > 50000 || !line){
        qDebug() << "";
    }

    return pointer;
}

void WMalloc_private_new(cchar *function, cchar *file, unsigned long int line, cvoid *pointer)
{
    mem_info mem;
    strncpy(mem.file,       file,       sizeof(mem.file));
    strncpy(mem.function,   function,   sizeof(mem.function));
    mem.line = line;

    if(line > 50000 || !line){
        return;
    }

    mem.pointer = pointer;

    {
        WMutexLocker _(_mem_mutex);
        _mem->append(mem);
    }
}

static not_used void print_all_mem(void)
{
    if(likely(_mem->isEmpty())){
        qDebug() << "No mem leak";
        return;
    }

    qDebug() << "Mem leak";

    for(const auto &tmp : std::as_const(*_mem)){
        print_mem_info(&tmp);
    }
}

void WFree_private(cvoid *mem, const char *file, const char *function)
{
    int i;
    const mem_info *tmp;
    WListFast<mem_info> &__mem = *_mem;
    int res = 0;
    QString msg;

    if(un(!mem))
        return;

    _mem_mutex.lock();

    i = _mem->size();

    for(i --; i >= 0; i--){
        tmp = &__mem.at(i);

        if(tmp->pointer == mem)
        {
            __mem.remove(i);
            res = 1;
            goto out;
        }
    }

out:
    _mem_mutex.unlock();

    if(un(!res)){
        msg = QString("Mem free not record. Pointer %1 File %2 Function %3").arg(QString::number((quint64)mem), file, function);
        qDebug() << msg;
        std::abort();
    }
}

void WEnd_application(void)
{
    if(likely(_mem->isEmpty())){
        qDebug() << "No mem leak";
        return;
    }

    qDebug() << "Mem leak";

    for(const auto &tmp : std::as_const(*_mem)){
        print_mem_info(&tmp);
    }
}

#endif
