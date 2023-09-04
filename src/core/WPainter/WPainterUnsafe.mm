#include "WPainterUnsafe.h"

#include "core/Image/WImage.h"
#include "core/Image/WImagePrivate.h"

#import <CoreFoundation/CoreFoundation.h>
#import <UIKit/UIKit.h>

class WPainterUnsafePrivate
{
public:
    WPainterUnsafePrivate(CGContextRef ref) : context(ref) { W_ASSERT(context != nil); }
    CGContextRef context;
};

static auto getAdaptCompositionMode (WPainter::CompositionMode compositionMode) -> CGBlendMode
{
    switch (compositionMode)
    {
        case WPainter::CompositionMode::CompositionMode_Clear:
            return CGBlendMode::kCGBlendModeClear;
        case WPainter::CompositionMode::CompositionMode_SourceOver:
            return CGBlendMode::kCGBlendModeSourceAtop;
        case WPainter::CompositionMode::CompositionMode_DestinationOver:
            return CGBlendMode::kCGBlendModeDestinationOver;
    }
    W_ASSERT_TEXT(0, "Composition mode not implemented");
}

WPainterUnsafe::WPainterUnsafe() noexcept
    : _dataPrivate(nullptr)
{
    
}

void WPainterUnsafe::execute(const std::function<void()> &function)
{
    auto realCompositionMode = getAdaptCompositionMode(_compositionMode);
    
    //W_ASSERT(this->_dataPrivate->context == UIGraphicsGetCurrentContext());
    W_ASSERT(_target != nullptr);
    
    dispatch_block_t realMethod = ^{
        @autoreleasepool {
            // Crea un renderer di immagini con le opzioni desiderate
            CGContextRef context = UIGraphicsGetCurrentContext();

            CGContextSetShouldAntialias(context, YES);
            CGContextSetLineCap(context, kCGLineCapRound);
            CGContextSetLineJoin(context, kCGLineJoinRound);
            CGContextSetBlendMode(context, realCompositionMode);
            CGContextSetLineWidth(context, _pen.widthF());
            
            function();
        }
    };
    
    if (![NSThread isMainThread])
        dispatch_sync(dispatch_get_main_queue(), realMethod);
    else
        realMethod();
}

bool WPainterUnsafe::begin(WImage *pixmap)
{
    WMutexLocker _(this->_lock);
    W_ASSERT_TEXT(pixmap != nullptr, "Pixmap passed as target is null");
    W_ASSERT_TEXT(!isActive(), "Trying to begin on an WPainter not ended");
    
    this->_target = pixmap;

    UIGraphicsBeginImageContext(CGSizeMake(this->width(), this->height()));
    //W_ASSERT(_dataPrivate == nil);
    //this->_dataPrivate = new WPainterUnsafePrivate(UIGraphicsGetCurrentContext());
    
    /*
    UIGraphicsImageRendererFormat *format = [UIGraphicsImageRendererFormat defaultFormat];
    format.scale = 1.0;
    format.opaque = NO;
    format.preferredRange = UIGraphicsImageRendererFormatRangeStandard;
    */
    
    [_target->_d->image drawAtPoint:CGPointZero];
    
    return true;
}

auto WPainterUnsafe::end() -> bool
{
    //W_ASSERT(_dataPrivate != nullptr);
    
    _target->_d->image = UIGraphicsGetImageFromCurrentImageContext();
    UIGraphicsEndImageContext();
    
    //_dataPrivate->context = nil;
    //delete this->_dataPrivate;
    
    _target = nullptr;
    return true;
}

auto WPainterUnsafe::move(const PointF &point) -> void
{
    CGContextRef context = UIGraphicsGetCurrentContext();
    CGContextMoveToPoint(context, point.x(), point.y());
}

void WPainterUnsafe::addCurve (const PointF &to, const PointF &controll)
{
    CGContextRef context = UIGraphicsGetCurrentContext();
    CGContextAddQuadCurveToPoint(context, controll.x(), controll.y(), to.x(), to.y());
}

void WPainterUnsafe::closePath ()
{
    CGContextRef context = UIGraphicsGetCurrentContext();
    CGContextStrokePath(context);
}
