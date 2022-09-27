#pragma once

#include "core/WZipWriterSingle.h"
#include "utils/common_script.h"
#include "utils/common_error_definition.h"
#include "core/WZipReaderSingle.h"

class stroke_file
{
#ifdef ALL_VERSION
    static int load_ver_0(class stroke &_stroke, WZipReaderSingle &reader);
    static int load_ver_1(class stroke &_stroke, WZipReaderSingle &reader);
#endif // ALL_VERSION

    static int load_ver_2(class stroke &_stroke, WZipReaderSingle &reader);
public:

#ifdef ALL_VERSION
    static int load(class stroke &_stroke, int version, WZipReaderSingle &reader);
#endif // ALL_VERSION

    static size_t get_size_in_file(const stroke &_stroke);

    static int save(const class stroke &_stroke, WZipWriterSingle &file);
};

