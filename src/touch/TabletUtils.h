#pragma once

#include "touch/dataTouch/Point.h"
#include <QPen>
#include "currenttitle/document.h"
#include "touch/laser/Laser.h"
#include "utils/Optional.h"


class TabletUtils {
private:
    const bool _withPdf;
    const bool _isExportingPdf;
    const std::function<bool()> &_isPlay;
    const std::function<int ()> &_positionAudio;
    double _m;
    Optional<Laser> _laser;
    QPen _pen;
    QRectF _visibleArea;

    const Document &_doc;
    QPainter *_painter;

public:
    struct DataPaint{
        bool withPdf;
        bool IsExportingPdf;
        const std::function<bool()> &isPlay;
        const std::function<int()>  &positionAudio;
        double m;
        Optional<Laser> laser;

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

    explicit TabletUtils(QPainter &painter, const std::function<bool()> &isPlay,
                         const std::function<int()> &positionAudio,
                         double m, Optional<Laser> laser, const Document &doc,
                         bool withPdf, bool isExporting, const QRectF &visibleArea);
    ~TabletUtils() = default;

    constexpr int getTime() const;
    constexpr bool withPdf() const;
    constexpr void setPainter(QPainter &painter);
    constexpr double getZoom() const;

    static double pressureToWidth(double val);
    void load();
private:
    void loadLaser();

    void drawForAudio();

    constexpr Laser &getLaser();
    constexpr QPainter &getPainter();
};

inline TabletUtils::TabletUtils(QPainter &painter, const std::function<bool()> &isPlay, const std::function<int()> &positionAudio, double m, Optional<Laser> laser, const Document &doc, bool withPdf, bool isExporting, const QRectF &visibleArea)
    : _withPdf(withPdf)
    , _isExportingPdf(isExporting)
    , _doc(doc)
    , _isPlay(isPlay)
    , _m(m)
    , _painter(&painter)
    , _positionAudio(positionAudio)
{
    if (this->_isExportingPdf) {
        this->_visibleArea = QRectF {
            0., 0.,
                Page::getWidth(),
                Page::getHeight() * this->_doc.lengthPage()
        };
    } else {
        _visibleArea = visibleArea;
    }
}

inline constexpr QPainter &TabletUtils::getPainter()
{
    return *this->_painter;
}

inline constexpr Laser& TabletUtils::getLaser()
{
    W_ASSERT(this->_laser.has_value());
    return *this->_laser;
}

inline double TabletUtils::pressureToWidth(double val)
{
    return val * 10 + 1;
}

inline constexpr void TabletUtils::setPainter(QPainter &painter)
{
    W_ASSERT(painter.isActive());
    this->_painter = &painter;
}

inline constexpr double TabletUtils::getZoom() const
{
    return this->_doc.getZoom();
}

inline constexpr int TabletUtils::getTime() const
{
    return this->_positionAudio();
}
