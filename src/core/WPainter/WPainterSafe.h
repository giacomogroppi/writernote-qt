#pragma once

#include "WPainter.h"

class WPainterSafe final: public WPainter
{
protected:
    void execute(const std::function<void()> &function) final;
public:
    WPainterSafe() noexcept = default;
    ~WPainterSafe() final = default;

    bool begin(WImage *pixmap) final;
    bool end() final;
};
