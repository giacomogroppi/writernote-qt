#pragma once

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
              cbool withPdf);

public:
    topdf(const QString &path, const Document &doc);
    ~topdf();

    bool createpdf(cbool withPdf);

};

inline void topdf::draw(QPainter &painter, double m, cbool withPdf)
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

