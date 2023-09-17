#pragma once

#include "WPainter.h"
#include "core/pointer/UniquePtr.h"
#include "utils/platform.h"

#if is_ios
/**
  * TODO: translate to english
  * Questa classe permette di mantenere il contesto attivo da quando si chiama begin() fino a quando
  * si chiama end(), è possibile creare nel mentre altri WPainterUnsafe, ma è necessario che
  * tutti i WPainterUnsafe o WPainterSafe creati nel mezzo terminino, per questo non è una buona idea utilizzare
  * il multithread con questi oggetti.
 */
class WPainterUnsafe final: public WPainter
{
protected:
    class WPainterUnsafePrivate* _dataPrivate;
    void execute(const Fn<void()> &function) final;
public:
    WPainterUnsafe() noexcept;
    ~WPainterUnsafe() final = default;

    void move (const PointF &point) final;
    
    void addCurve(const PointF &to, const PointF &control) final;
    void closePath () final;
    
    bool begin(WImage *pixmap) final;
    bool end() final;
};
#elif is_android

#else
using WPainterUnsafe = WPainter;
#endif
