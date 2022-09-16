#pragma once

#include <QImage>

class Document;
class TabletCanvas;

class preview
{
public:
    static void get(QPixmap &ref, const Document &doc, const bool withPdf, const int width);
    static bool get(QPixmap &ref, const bool withPdf, const QString &path, const int height, const int width);

};
