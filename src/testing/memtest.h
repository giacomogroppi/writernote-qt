#ifndef MEMTEST_H
#define MEMTEST_H

#include "utils/common_script.h"

//#define MEMTEST
// debug for memory
#ifdef MEMTEST

void WMalloc_private(size_t size, const char *file, const char *function);
void WFree_private(void *mem);

void WEnd_application(void);

# define WMalloc(size) \
    malloc(size); \
    WMalloc_private(__FUNCTION__, __FILE__)

# define WFree(mem) \
    free(mem) \
    WFree_private(mem, __FUNCTION__, __FILE__)

# define WNew(type, arg) \
    new type(arg); \
    WMalloc_private(__FUNCTION__, __FILE__)

# define WDelete(pointer) \
    delete pointer; \
    WFree_private(pointer, __FUNCTION__, __FILE__)

#else
# define WMalloc(size) malloc(size)
# define WFree(mem) free(mem)
#endif

#endif // MEMTEST_H
