#pragma once

#include "touch/dataTouch/Point.h"
#include <QPen>
#include "currenttitle/document.h"
#include "touch/laser/Laser.h"

namespace drawUtils{
    void loadSingleSheet(
            QPainter &painter,  const Page &page,
            cdouble zoom,       cdouble delta,
            QPen &_pen,         const QPointF& pointFirstPage);
}

class TabletUtils {
public:
    struct DataPaint{
        bool withPdf;
        bool IsExportingPdf;
        std::function<bool()> isPlay;
        std::function<int()> positionAudio;
        double m;
        std::optional<Laser> laser;
        QPixmap *m_pixmap;

        QPen pen;
        Point lastPoint;

        void reset()
        {
            this->m = 1.0;
            this->withPdf = true;
        }
#define DATAPAINT_DEFINEREST \
    .pen = QPen(), \
    .lastPoint = Point()
    };

    static double pressureToWidth(double val);
    static void load(QPainter &painter, const Document &data, DataPaint &dataPoint);
};

inline double TabletUtils::pressureToWidth(double val)
{
    return val * 10 + 1;
}

