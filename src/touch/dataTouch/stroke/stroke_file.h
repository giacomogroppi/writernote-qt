#pragma once

#include "FileContainer/WZipWriterSingle.h"
#include "utils/WCommonScript.h"
#include "utils/common_error_definition.h"
#include "FileContainer/WZipReaderSingle.h"

class stroke_file
{
    static int load_ver_0(class Stroke &_stroke, WZipReaderSingle &reader);
    static int load_ver_1(class Stroke &_stroke, WZipReaderSingle &reader);
    static int load_ver_2(class Stroke &_stroke, WZipReaderSingle &reader);
public:
};

