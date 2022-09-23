#pragma once

#include "core/WZipWriterSingle.h"
#include "utils/common_script.h"
#include "zip.h"

class page_file
{
#ifdef ALL_VERSION
    static int load_ver_0(class page &_page, zip_file_t *file);
    static int load_ver_1(class page &_page, zip_file_t *file);
    static int load_ver_2(class page &_page, zip_file_t *file);
#endif
    static int load_ver_2(class page &_page, class WReadZip &reader, int id);
public:

#ifdef ALL_VERSION
    static int load(class page &_page, int ver_stroke, zip_file_t *file);
#endif // ALL_VERSION

    static int load(class page &_page, int ver_stroke, class WReadZip &readZip, int id);
    static size_t size_in_file(const page &_page, cbool saveImg);

    static int save(const page *_page, WZipWriterSingle &writer, cbool saveImg);
};

