#ifndef MEMTEST_H
#define MEMTEST_H

#include "utils/common_script.h"

// debug for memory
#ifdef DEBUG_CORE

void WMalloc_private(const char *file, const char *function, const void *pointer);
void WFree_private(void *mem, const char *file, const char *function);

void WEnd_application(void);

# define WMalloc(pointer, size) \
    do{ \
        pointer = (typeof(pointer))malloc(size); \
        WMalloc_private(__FUNCTION__, __FILE__, pointer); \
    }while(0);

# define WFree(mem) \
    do{ \
        free(mem) \
        WFree_private(__FUNCTION__, __FILE__, mem); \
    }while(0);

# define WNew(pointer, type, arg) \
    do{ \
        pointer = new type arg; \
        WMalloc_private(__FUNCTION__, __FILE__, pointer); \
    }while(0);

# define WDelete(pointer) \
    do{ \
        WFree_private(pointer, __FUNCTION__, __FILE__); \
        delete pointer; \
    }while(0);

#else
# define WMalloc(pointer, size) do{ pointer = malloc(size); } while(0);
# define WFree(mem) do{ free(mem); } while(0);
# define WNew(pointer, type, arg) do{ pointer = new type arg; } while(0);
# define WDelete(pointer) do{ delete pointer; } while(0);
#endif

#endif // MEMTEST_H
