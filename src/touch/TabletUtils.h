#pragma once

#include "touch/dataTouch/Point.h"
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
    WPen _pen;
    RectF _visibleArea;

    const Document &_doc;
    WPainter *_painter;

public:
    struct DataPaint{
        bool withPdf;
        bool IsExportingPdf;
        const std::function<bool()> &isPlay;
        const std::function<int()>  &positionAudio;
        double m;
        Optional<Laser> laser;

        WPen pen;
        WPoint lastPoint;

        void reset()
        {
            this->m = 1.0;
            this->withPdf = true;
        }
#define DATAPAINT_DEFINEREST \
    .pen = WPen(), \
    .lastPoint = Point()
    };

    explicit TabletUtils(WPainter &painter, const std::function<bool()> &isPlay,
                         const std::function<int()> &positionAudio,
                         double m, Optional<Laser> laser, const Document &doc,
                         bool withPdf, bool isExporting, const RectF &visibleArea);
    ~TabletUtils() = default;

    int getTime() const;
    constexpr bool withPdf() const;
    void setPainter(WPainter &painter) noexcept;
    constexpr double getZoom() const;

    static double pressureToWidth(double val);
    void load();
private:
    void loadLaser();

    void drawForAudio();

    constexpr Laser &getLaser();
    constexpr WPainter &getPainter();
};

inline TabletUtils::TabletUtils(WPainter &painter, const std::function<bool()> &isPlay, const std::function<int()> &positionAudio, double m, Optional<Laser> laser, const Document &doc, bool withPdf, bool isExporting, const RectF &visibleArea)
    : _withPdf(withPdf)
    , _isExportingPdf(isExporting)
    , _isPlay(isPlay)
    , _positionAudio(positionAudio)
    , _m(m)
    , _laser(laser)
    , _visibleArea(visibleArea)
    , _doc(doc)
    , _painter(&painter)
{
    if (this->_isExportingPdf) {
        this->_visibleArea = RectF {
            0., 0.,
                Page::getWidth(),
                Page::getHeight() * this->_doc.lengthPage()
        };
    } else {
        _visibleArea = visibleArea;
    }
}

inline constexpr WPainter &TabletUtils::getPainter()
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

inline void TabletUtils::setPainter(WPainter &painter) noexcept
{
    W_ASSERT(painter.isActive());
    this->_painter = &painter;
}

inline constexpr double TabletUtils::getZoom() const
{
    return this->_doc.getZoom();
}

inline int TabletUtils::getTime() const
{
    return this->_positionAudio();
}
