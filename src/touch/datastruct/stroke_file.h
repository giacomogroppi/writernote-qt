#ifndef STROKE_FILE_H
#define STROKE_FILE_H

#include "utils/common_script.h"
#include "utils/common_error_definition.h"
#include "zip.h"
#include "datawrite/source_read_ext.h"

class stroke_file
{
    static int load_ver_0(class stroke &_stroke, zip_file_t *file);
    static int load_ver_1(class stroke &_stroke, zip_file_t *file);
    static int load_ver_2(class stroke &_stroke, zip_file_t *file);

public:
    static int load(class stroke &_stroke, int version, zip_file_t *file);
    static int save(const class stroke &_stroke, zip_source_t *file);
};

#endif // STROKE_FILE_H
