#include "WPainter.h"
#include "core/Image/WImage.h"
#include "core/Image/WImagePrivate.h"

#import <CoreFoundation/CoreFoundation.h>
#import <Cocoa/Cocoa.h>

auto getAdaptCompositionMode (WPainter::CompositionMode compositionMode) -> NSCompositingOperation
{
    switch (compositionMode)
    {
        case WPainter::CompositionMode::CompositionMode_Clear:
            return NSCompositingOperation::NSCompositingOperationClear;
        case WPainter::CompositionMode::CompositionMode_SourceOver:
            return NSCompositingOperation::NSCompositingOperationSourceOver;
        case WPainter::CompositionMode::CompositionMode_DestinationOver:
            return NSCompositingOperation::NSCompositingOperationDestinationOver;
    }
    W_ASSERT_TEXT(0, "Composition mode not implemented");
}

auto createNSColor (const WColor &color) -> NSColor*
{
    return [NSColor
                colorWithRed:color.getRed()
                green:color.getGreen()
                blue:color.getBlue()
                alpha:color.getAlfa()];
}

WPainter::WPainter() noexcept
    : _target(nullptr)
    , _isAntialeasing(false)
    , _compositionMode(WPainter::CompositionMode::CompositionMode_SourceOver)
{
    
}

void WPainter::drawEllipse (const PointF &center, double rx, double ry)
{
    dispatch_async(dispatch_get_main_queue(), ^{
        WMutexLocker _(this->_lock);
        const NSImage *image = _target->_d->image;
        const auto realCompositionMode = getAdaptCompositionMode(this->_compositionMode);
        const double width = this->_pen.widthF();
        NSColor *color = createNSColor(this->_color);
        
        [NSBezierPath setDefaultLineWidth:width];
        
        [color set];
        
        [[NSGraphicsContext currentContext] setCompositingOperation:realCompositionMode];
        
        [image lockFocus];
        
        // set the width
        [NSBezierPath setDefaultLineWidth:width];
        
        // set the color
        [color set];
        
        // Disegna l'ellisse, specificando il rettangolo in cui disegnarlo
        NSRect ellipseRect = NSMakeRect(center.x() - rx, center.y() - ry, center.x() + rx, center.y() + ry);
        NSBezierPath *ellipsePath = [NSBezierPath bezierPathWithOvalInRect:ellipseRect];
        [ellipsePath stroke];
        
        // Fine del disegno, sblocca il contesto
        [image unlockFocus];
        
    });
}

void WPainter::drawImage (const RectF &target, const WImage &image, const RectF &source)
{
    dispatch_async(dispatch_get_main_queue(), ^{
        WMutexLocker _(this->_lock);
        const NSImage *imageSource = image._d->image;
        const NSImage *imageTarget = _target->_d->image;
        const auto realCompositionMode = getAdaptCompositionMode(this->_compositionMode);
        
        NSRect targetRect = NSMakeRect(
                                       target.topLeft().x(),        target.topLeft().y(),
                                       target.bottomRight().x(),    target.bottomRight().y());
        
        [imageTarget drawInRect:targetRect];
        
        NSRect sourceRect = NSMakeRect (source.topLeft().x(),       source.topLeft().y(),
                                        source.bottomRight().x(),   source.bottomRight().y());
        
        [imageSource drawInRect:sourceRect fromRect:NSZeroRect operation:realCompositionMode fraction:1.0];
        
        [imageTarget unlockFocus];
    });
}

void WPainter::drawLine(int x1, int y1, int x2, int y2)
{
    dispatch_async(dispatch_get_main_queue(), ^{
        WMutexLocker _(this->_lock);
        const NSImage *image = _target->_d->image;
        const auto realCompositionMode = getAdaptCompositionMode(this->_compositionMode);
        const double width = this->_pen.widthF();
        NSColor *color = createNSColor(this->_color);
        
        // Imposta lo spessore della linea
        [NSBezierPath setDefaultLineWidth:width];

        // Imposta il colore della linea
        [color set];

        [[NSGraphicsContext currentContext] setCompositingOperation:realCompositionMode];
        
        // Disegna la semiretta, specificando i punti di inizio e fine
        NSPoint startPoint = NSMakePoint(x1, y1); // Punto di inizio della semiretta
        NSPoint endPoint = NSMakePoint(x2, y2); // Punto di fine della semiretta

        // Crea il percorso della semiretta e disegnala
        NSBezierPath *linePath = [NSBezierPath bezierPath];
        [linePath moveToPoint:startPoint];
        [linePath lineToPoint:endPoint];
        [linePath stroke];

        // Fine del disegno, sblocca il contesto
        [image unlockFocus];
    });
}

void WPainter::drawPixmap(const RectF &target, const WPixmap& pixmap, const RectF &source)
{
    const WImage &image = static_cast<const WImage&>(pixmap);
    return this->drawImage(target, image, source);
}

void WPainter::drawPixmap  (const RectF &target, const WPixmap &pixmap)
{
    const RectF source = pixmap.rect();
    return this->drawPixmap(target, pixmap, source);
}

void WPainter::drawPixmap  (const WRect &target, const WPixmap &pixmap)
{
    const RectF source = pixmap.rect().castTo<double>();
    const RectF targetCasted = target.castTo<double>();
    return this->drawPixmap(targetCasted, pixmap, source);
}

void WPainter::drawPixmap  (const WRect &target, const WPixmap &pixmap, const WRect &source)
{
    const RectF sourceCasted = source.castTo<double>();
    const RectF targetCasted = target.castTo<double>();
    return this->drawPixmap(targetCasted, pixmap, sourceCasted);
}

void WPainter::setColor(const WColor &color)
{
    WMutexLocker _(this->_lock);
    this->_color = color;
}

void WPainter::setPen(const WPen &pen)
{
    WMutexLocker _(this->_lock);
    this->_pen = pen;
}

void WPainter::setAntialeasing()
{
    WMutexLocker _(this->_lock);
    this->_isAntialeasing = true;
}

auto WPainter::compositionMode() const -> WPainter::CompositionMode
{
    WMutexLocker _(this->_lock);
    return this->_compositionMode;
}

void WPainter::drawLine (const PointF &p1, const PointF &p2)
{
    return this->drawLine (p1.x(), p1.y(), p2.x(), p2.y());
}

void WPainter::setCompositionMode(enum CompositionMode compositionMode)
{
    WMutexLocker _(this->_lock);
    this->_compositionMode = compositionMode;
}

bool WPainter::begin(WImage *pixmap)
{
    WMutexLocker _(this->_lock);
    W_ASSERT_TEXT(this->_target == nullptr, "Trying to begin on an WPainter not ended");
    this->_target = pixmap;
    return true;
}
