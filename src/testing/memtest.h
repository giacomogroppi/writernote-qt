#pragma once

#include "utils/common_script.h"

#ifdef DEBUGINFO
#   define DEBUG_MEM
#endif

// debug for memory
#ifdef DEBUG_MEM

void *WMalloc_private(      cchar *function, cchar *file, unsigned long int line, size_t size);
void WMalloc_private_new(   cchar *function, cchar *file, unsigned long int line, cvoid *pointer);

void WFree_private(cvoid *mem, const char *file, const char *function);

void WEnd_application();

# define WMalloc(size) WMalloc_private(__FUNCTION__, __FILE__, __LINE__, (size));

# define WFree(mem) \
    do{ \
        WFree_private(mem, __FILE__, __FUNCTION__); \
        free(mem); \
    }while(0);


// like this: WNew(m_img, fromimage, (this));

# define WNew(pointer, type, arg) \
    do{ \
        pointer = new type arg; \
        WMalloc_private_new(__FUNCTION__, __FILE__, __LINE__, pointer); \
    }while(0);

# define WDelete(pointer) \
    do{ \
        WFree_private(pointer, __FUNCTION__, __FILE__); \
        delete pointer; \
    }while(0);

#else // DEBUG_MEM
# define WMalloc(size) malloc(size)
# define WFree(mem) free(mem)
# define WNew(pointer, type, arg) do{ pointer = new type arg; } while(0);
# define WDelete(pointer) do{ delete pointer; } while(0);
force_inline void WEnd_application(void) {}
#endif // DEBUG_MEM

