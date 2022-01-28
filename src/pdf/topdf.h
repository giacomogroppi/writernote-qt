#ifndef TOPDF_H
#define TOPDF_H
#include "touch/datastruct/datastruct.h"
#include "mainwindow.h"
#include "touch/tabletcanvas.h"

class topdf
{
private:
    Document *data = nullptr;
    const QString *path;

    void translate();

    void draw(QPainter &painter, double m,
              const bool withPdf);

public:
    topdf(const QString &path, const Document &doc);
    ~topdf();

    bool createpdf(const bool withPdf);

};

inline void topdf::draw(QPainter &painter, double m, const bool withPdf)
{
    DataPaint dataPaint = {
        .withPdf = withPdf,
        .IsExportingPdf = true,
        .m = m,
        .parent = nullptr,
        .m_pixmap = nullptr,
        DATAPAINT_DEFINEREST
    };
    TabletCanvas::load(painter, this->data, dataPaint);
}

#endif // TOPDF_H
