#pragma once

#include "core/WZipWriterSingle.h"
#include "utils/common_script.h"
#include "utils/common_error_definition.h"
#include "zip.h"

class stroke_file
{
#ifdef ALL_VERSION
    static int load_ver_0(class stroke &_stroke, zip_file_t *file);
    static int load_ver_1(class stroke &_stroke, zip_file_t *file);
    static int load_ver_2(class stroke &_stroke, zip_file_t *file);
#endif // ALL_VERSION
    static int load_ver_2(class stroke &_stroke, class WReadZip &reader, int id);
public:

#ifdef ALL_VERSION
    static int load(class stroke &_stroke, int version, zip_file_t *file);
#endif // ALL_VERSION
    static int load(class stroke &_stroke, int version, WReadZip &reader, int id);


    static size_t get_size_in_file(const stroke &_stroke);

    static int save(const class stroke &_stroke, WZipWriterSingle &file);
};

