#include "WPainterSafe.h"
#import <UIKit/UIKit.h>
#include "core/Image/WImage.h"
#include "core/Image/WImagePrivate.h"

#import <CoreFoundation/CoreFoundation.h>

/**
  * TODO: create header file for common functions between two painter classes
 */
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

void WPainterSafe::execute(const std::function<void()> &function)
{
    auto realCompositionMode = getAdaptCompositionMode(_compositionMode);
    
    W_ASSERT(_target != nullptr);
    
    dispatch_block_t realMethod = ^{
        @autoreleasepool {
            UIGraphicsBeginImageContextWithOptions(CGSizeMake(width(), height()), false, 1.0);
            
            // Qui puoi eseguire il disegno dell'immagine
            CGContextRef context = UIGraphicsGetCurrentContext();
                
            // Esempio: Disegna un cerchio con anti-aliasing abilitato
            CGContextSetShouldAntialias(context, YES);
            CGContextSetLineCap(context, kCGLineCapRound);
            CGContextSetBlendMode(context, realCompositionMode);
            CGContextSetLineWidth(context, _pen.widthF());
            [_target->_d->image drawAtPoint:CGPointZero];
            _target->_d->image = nil;
                
            if (this->_needToMove) {
                CGContextMoveToPoint(context, _pointMove.x(), _pointMove.y());
                _needToMove = false;
            }
                
            if (this->_needToCurve) {
                CGContextAddQuadCurveToPoint(context, _controll.x(), _controll.y(), _to.x(), _to.y());
                this->_needToCurve = false;
            }
                
            function();
            
            
            _target->_d->image = UIGraphicsGetImageFromCurrentImageContext();
            UIGraphicsEndImageContext();
        }
    };
    
    if (![NSThread isMainThread])
        dispatch_sync(dispatch_get_main_queue(), realMethod);
    else
        realMethod();
}

bool WPainterSafe::begin(WImage *pixmap)
{
    WMutexLocker _(this->_lock);
    W_ASSERT_TEXT(pixmap != nullptr, "Pixmap passed as target is null");
    W_ASSERT_TEXT(!isActive(), "Trying to begin on an WPainter not ended");
    
    this->_target = pixmap;

    return true;
}

bool WPainterSafe::end()
{
    _target = nullptr;
    
    return true;
}

void WPainterSafe::move(const PointF& point)
{
    this->_needToMove = true;
    this->_pointMove = point;
}

void WPainterSafe::addCurve(const PointF &to, const PointF &controll)
{
    W_ASSERT(_needToCurve == false);
    W_ASSERT(_needToMove == true);
    _needToCurve = true;
    _to = to;
    _controll = controll;
}

void WPainterSafe::closePath ()
{
    W_ASSERT(_needToMove == true);
    W_ASSERT(_needToCurve == true);
    
    this->execute([this] {
        WMutexLocker _(this->_lock);
        
        CGContextRef context = UIGraphicsGetCurrentContext();
        CGContextStrokePath(context);
    });
}
