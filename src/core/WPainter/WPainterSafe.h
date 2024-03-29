#pragma once

#include "WPainter.h"
#include "utils/platform.h"

#if is_ios
class WPainterSafe final: public WPainter
{
protected:
    void execute(const Fn<void()> &function) final;
public:
    WPainterSafe() noexcept = default;
    ~WPainterSafe() final = default;

    WPainterSafe(const WPainterSafe& other) = delete;
    WPainterSafe(WPainterSafe &&other) = default;
    
    void move (const PointF& point) final;
    void addCurve (const PointF &to, const PointF &control) final;
    void closePath () final;
    
    bool begin(WImage *pixmap) final;
    bool end() final;

    auto operator=(const WPainterSafe &other) -> WPainterSafe& = delete;
    auto operator=(WPainterSafe &&other) -> WPainterSafe& = delete;
private:
    bool _needToMove = false;
    PointF _pointMove;
    
    bool _needToCurve = false;
    PointF _to, _controll;
};
#elif is_android
#else
using WPainterSafe = WPainter;
#endif