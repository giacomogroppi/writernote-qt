#pragma once

#include "touch/dataTouch/datastruct/DataStruct.h"
#include "touch/TabletUtils.h"

class topdf
{
private:
    class Document *data = nullptr;
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
    TabletUtils::DataPaint dataPaint = {
        .withPdf = withPdf,
        .IsExportingPdf = true,
        .m = m,
        .m_pixmap = nullptr,
        DATAPAINT_DEFINEREST
    };
    TabletUtils::load(painter, *data, dataPaint);
}

