#pragma once

#include "core/Image/WImage.h"
#include "utils/common_def.h"
#include "core/Pixmap/WPixmap.h"
#include "core/Path/WPath.h"

class Document;
class TabletCanvas;

class preview
{
public:
    static void get(WPixmap &ref, const Document &doc, bool withPdf, int width);
    static bool get(WPixmap &ref, bool withPdf, const WPath &path, int height, int width);

};
