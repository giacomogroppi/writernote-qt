#pragma once

#include "core/WZipWriterSingle.h"
#include "utils/common_script.h"
#include "utils/common_error_definition.h"
#include "core/WZipReaderSingle.h"

class stroke_file
{
#ifdef ALL_VERSION
    static int load_ver_0(class Stroke &_stroke, WZipReaderSingle &reader);
    static int load_ver_1(class Stroke &_stroke, WZipReaderSingle &reader);
#endif // ALL_VERSION

    static int load_ver_2(class Stroke &_stroke, WZipReaderSingle &reader);
public:

#ifdef ALL_VERSION
    static int load(class Stroke &_stroke, int version, WZipReaderSingle &reader);
#endif // ALL_VERSION

    static size_t get_size_in_file(const Stroke &_stroke);

    static int save(const class Stroke &_stroke, WZipWriterSingle &file);
};

