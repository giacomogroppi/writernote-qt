#pragma once

#include "core/WImage.h"
#include "utils/common_def.h"

class Document;
class TabletCanvas;

class preview
{
public:
    static void get(QPixmap &ref, const Document &doc, cbool withPdf, const int width);
    static bool get(QPixmap &ref, cbool withPdf, const WByteArray &path, const int height, const int width);

};
