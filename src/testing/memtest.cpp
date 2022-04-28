#include "memtest.h"
#include <QList>
#include <QVector>
#include "string.h"
#include "pthread.h"

#ifdef DEBUG_MEM

struct mem_info
{
    const void *pointer;
    char function[128];
    char file[128];
    unsigned long int line;
};

static pthread_mutex_t _mem_mutex;
static QList<mem_info> *_mem;

void __init__ initMem(void)
{
    qDebug() << "Memtest enable";
    pthread_mutex_init(&_mem_mutex, NULL);
    _mem = new QList<mem_info>();
}

static void print_mem_info(const mem_info *mem)
{
    assert(mem);
    QString message;

    if(mem->line > 50000 || !mem->line){
        return;
    }

    message = QString("Pointer %1 File: %2 Function %3 Line %4").arg(qstr::number((quint64)mem->pointer, 16),
                                                                         mem->file,
                                                                         mem->function,
                                                                         qstr::number(mem->line));

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

    pthread_mutex_lock(&_mem_mutex);
    _mem->append(mem);
    pthread_mutex_unlock(&_mem_mutex);

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

    pthread_mutex_lock(&_mem_mutex);
    _mem->append(mem);
    pthread_mutex_unlock(&_mem_mutex);
}

static not_used void print_all_mem(void)
{
    if(likely(_mem->isEmpty())){
        qDebug() << "No mem leak";
        return;
    }

    qDebug() << "Mem leak";

    for(const auto &tmp : qAsConst(*_mem)){
        print_mem_info(&tmp);
    }
}

void WFree_private(cvoid *mem, const char *file, const char *function)
{
    int i;
    const mem_info *tmp;
    QList<mem_info> &__mem = *_mem;
    int res = 0;
    QString msg;

    if(unlikely(!mem))
        return;

    pthread_mutex_lock(&_mem_mutex);

    i = _mem->length();

    for(i --; i >= 0; i--){
        tmp = &__mem.at(i);

        if(tmp->pointer == mem)
        {
            __mem.removeAt(i);
            res = 1;
            goto out;
        }
    }

out:
    pthread_mutex_unlock(&_mem_mutex);

    if(unlikely(!res)){
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

    for(const auto &tmp : qAsConst(*_mem)){
        print_mem_info(&tmp);
    }
}

#endif
