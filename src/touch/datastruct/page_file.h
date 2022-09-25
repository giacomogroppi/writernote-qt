#pragma once

#include "core/WZipWriterSingle.h"
#include "utils/common_script.h"
#include "core/WZipReaderSingle.h"

class page_file
{
#ifdef ALL_VERSION
    static int load_ver_0(class page &_page, WZipReaderSingle &reader);
    static int load_ver_1(class page &_page, WZipReaderSingle &reader);
#endif // ALL_VERSION
    static int load_ver_2(class page &_page, WZipReaderSingle &reader);
public:

#ifdef ALL_VERSION
    static int load(class page &_page, int ver_stroke, WZipReaderSingle &reader);
#endif // ALL_VERSION

    static int load(class page &_page, int ver_stroke, class WReadZip &readZip, int id);
    static size_t size_in_file(const page &_page, cbool saveImg);

    static int save(const page *_page, WZipWriterSingle &writer, cbool saveImg);
};

