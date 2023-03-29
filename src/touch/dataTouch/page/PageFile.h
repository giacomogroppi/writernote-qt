#pragma once

#include "core/WZipWriterSingle.h"
#include "utils/WCommonScript.h"
#include "core/WZipReaderSingle.h"

class PageFile
{
    static int load_ver_0(class Page &_page, WZipReaderSingle &reader);
    static int load_ver_1(class Page &_page, WZipReaderSingle &reader);
    static int load_ver_2(class Page &_page, WZipReaderSingle &reader);
public:

    static int load(class Page &_page, int ver_stroke, WZipReaderSingle &reader);

    static int load(class Page &_page, int ver_stroke, class WReadZip &readZip, int id);
    static size_t size_in_file(const Page &_page, cbool saveImg);

    static int save(const Page *_page, WZipWriterSingle &writer, cbool saveImg);
};

