#include "source_read_ext.h"

short int source_read_ext(zip_file *file, void *data, int len){
    return - (zip_fread(file, data, len) == -1);
}

short int source_write_ext(zip_source_t *file, const void *data, int len){
    return - (zip_source_write(file, data, len) == 1);
}
