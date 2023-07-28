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
    
}

void WPainter::drawImage (const RectF &target, const WImage &image, const RectF &source)
{
    dispatch_async(dispatch_get_main_queue(), ^{
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
