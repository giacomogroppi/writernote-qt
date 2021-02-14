#ifndef SOURCE_READ_EXT_
#define SOURCE_READ_EXT_

#include "zip.h"

short int source_read_ext(zip_file *, void *, int);
short int source_write_ext(zip_source_t *, const void *, int);

#endif // ZIP_SOURCE_READ
