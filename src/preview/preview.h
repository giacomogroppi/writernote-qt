#ifndef PREVIEW_H
#define PREVIEW_H
#include <QImage>

class Document;
class TabletCanvas;

class preview
{
public:
    static void get(QImage &ref, const Document &doc, const bool withPdf);
    static bool get(QImage &ref, const bool withPdf, const QString &path);
    static void get_all(QList<QImage> &ref, Document &doc, const bool withPdf);

};

#endif // PREVIEW_H
