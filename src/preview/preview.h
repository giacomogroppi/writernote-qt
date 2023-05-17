#pragma once

#include "core/WImage.h"
#include "utils/common_def.h"
#include "core/WPixmap.h"

class Document;
class TabletCanvas;

class preview
{
public:
    static void get(WPixmap &ref, const Document &doc, cbool withPdf, const int width);
    static bool get(WPixmap &ref, cbool withPdf, const WByteArray &path, const int height, const int width);

};
