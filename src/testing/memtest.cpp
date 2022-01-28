#include "memtest.h"
#include <QList>
#include <QVector>
#include "string.h"
#include <malloc.h>

#ifdef MEMTEST

struct mem_info
{
    const void *pointer;
    char function[128];
    char file[128];
};

static QList<mem_info> _mem;

static void print_mem_info(const mem_info *mem)
{
    assert(mem);
    QString message;

    message = QString("Pointer %1 File: %2 Function %3").arg( QString::number((unsigned long)mem->pointer), mem->file, mem->function);

}

void WMalloc_private(const char *file, const char *function, const void *pointer)
{
    mem_info mem;

    strncpy(mem.file,       file,       sizeof(mem.file));
    strncpy(mem.function,   function,   sizeof(mem.function));

    mem.pointer = pointer;

    _mem.append(mem);
}

void WFree_private(void *mem)
{
    int i = _mem.length();
    const mem_info *tmp;
    QList<mem_info> &__mem = _mem;
    int res = 0;

    for(i --; i >= 0; i--){
        tmp = &__mem.at(i);
        if(tmp->pointer == mem){
            __mem.removeAt(i);
            res = 1;
            goto out;
        }
    }

out:
    if(unlikely(!res)){
        qDebug() << "mem free not record";
        print_mem_info((const mem_info *)mem);
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
