#ifndef SOURCE_READ_EXT_
#define SOURCE_READ_EXT_

#include "zip.h"
#include "utils/common_script.h"

short int source_read_ext(zip_file *, void *, int);
short int source_write_ext(zip_source_t *, const void *, int);

/* read */
#define SOURCE_READ_EXT(x, y, z) check+=zip_fread(x, y, z) == -1

#define ARGUMENT(x, y, z) if(unlikely(zip_fread(x, y,z)==-1))

#define SOURCE_READ_GOTO(x, y, z) ARGUMENT(x,y,z)goto free_;
#define SOURCE_READ_RETURN_SIZE(x, y, z) ARGUMENT(x, y, z)return ERROR;

/* write */
#define SOURCE_WRITE(x, y, z) \
    do{                                                                 \
        int ___res = zip_source_write(x, y, z);                         \
        if(unlikely(___res < 0 || ___res != (typeof(___res))z)){        \
            goto delete_;                                               \
        }                                                               \
    } while(0);

#define SAVE_BINARY(x, img) if(unlikely(salvabinario(x, img) == ERROR))goto delete_;

#define SOURCE_WRITE_RETURN_SIZE(x, y, z) \
    do{                                                             \
        int ___res = zip_source_write(x, y, z);                     \
        if(unlikely(___res < 0 || ___res != (typeof(___res))z)){    \
            return ERROR;                                           \
        }                                                           \
    } while(0);

#define SOURCE_WRITE_GOTO_SIZE(x, y, z) \
    do{                                                             \
        int ___res = zip_source_write(x, y, z);                     \
        if(unlikely(___res < 0 || ___res != (typeof(___res))z)){    \
            goto delete_;                                           \
        }                                                           \
    } while(0);

#endif // ZIP_SOURCE_READ
