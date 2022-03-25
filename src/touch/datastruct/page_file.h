#ifndef PAGE_FILE_H
#define PAGE_FILE_H

#include "utils/common_script.h"
#include "zip.h"

class page_file
{
    static int load_ver_0(class page &_page, zip_file_t *file);
    static int load_ver_1(class page &_page, zip_file_t *file);
    static int load_ver_2(class page &_page, zip_file_t *file);
public:
    static int load(class page &_page, int ver_stroke, zip_file_t *file);
    static int save(const page *_page, zip_source_t *file, cbool saveImg);
};

#endif // PAGE_FILE_H
