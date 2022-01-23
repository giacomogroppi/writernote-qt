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
#define SOURCE_READ_RETURN(x, y, z) ARGUMENT(x, y, z)return ERROR;

/* write */
#define SOURCE_WRITE(x, y, z) if(unlikely(zip_source_write(x, y, z) == -1)) goto delete_;
#define SAVE_BINARY(x) if(unlikely(salvabinario(x) == ERROR))goto delete_;

#define SOURCE_WRITE_RETURN(x, y, z) if(unlikely(zip_source_write(x, y, z) == -1)) return ERROR;

#endif // ZIP_SOURCE_READ
