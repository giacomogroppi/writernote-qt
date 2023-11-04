#pragma once

#include "touch/dataTouch/Point.h"
#include "currenttitle/document.h"
#include "touch/laser/Laser.h"
#include "utils/Optional.h"
#include "core/WFlags.h"
#include "touch/UpdateEvent.h"
#include "core/WElement.h"

class TabletUtils {
private:
    static constexpr auto debug = false;

    const Bool _withPdf;
    const Bool _isExportingPdf;
    const Bool _isPlay;
    const AudioPosition _positionAudio;
    Double _m;
    Optional<Laser> _laser;
    RectF _visibleArea;

    const Document &_doc;
    WPainter *_painter;

public:
    explicit TabletUtils(WPainter &painter, Bool isPlay, AudioPosition positionAudio,
                         Double m, Optional<Laser> laser, const Document &doc,
                         Bool withPdf, Bool isExporting, const RectF &visibleArea);
    ~TabletUtils() = default;

    [[nodiscard]]
    auto getTime() const -> AudioPosition;

    [[nodiscard]]
    constexpr auto withPdf() const -> Bool;

    void setPainter(WPainter &painter) noexcept;

    [[nodiscard]]
    constexpr auto getZoom() const -> double;

    static auto pressureToWidth(double val) -> double;

    using LoadTypes = WFlags<UpdateEvent::UpdateEventType>;

    void load(LoadTypes types);
private:
    void loadLaser();

    void drawForAudio();

    constexpr auto getLaser() -> Laser &;
    constexpr auto getPainter() -> WPainter &;
};

inline TabletUtils::TabletUtils(WPainter &painter, Bool isPlay, AudioPosition positionAudio,
                                Double m, Optional<Laser> laser,
                                const Document &doc, Bool withPdf, Bool isExporting,
                                const RectF &visibleArea)
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

inline auto TabletUtils::getTime() const -> AudioPosition
{
    return this->_positionAudio;
}
