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
            CGSize size = CGSizeMake(width(), height());
            
            // Crea un renderer di immagini con le opzioni desiderate
            UIGraphicsImageRendererFormat *format = [UIGraphicsImageRendererFormat defaultFormat];
            format.scale = 1;
            format.opaque = NO;
            format.preferredRange = UIGraphicsImageRendererFormatRangeStandard;

            UIGraphicsImageRenderer *renderer = [[UIGraphicsImageRenderer alloc] initWithSize:size format:format];

            _target->_d->image = [renderer imageWithActions:^(UIGraphicsImageRendererContext * _Nonnull rendererContext) {
                // Qui puoi eseguire il disegno dell'immagine
                CGContextRef context = rendererContext.CGContext;
                
                // Esempio: Disegna un cerchio con anti-aliasing abilitato
                CGContextSetShouldAntialias(context, YES);
                CGContextSetLineCap(context, kCGLineCapRound);
                CGContextSetBlendMode(context, realCompositionMode);
                [_target->_d->image drawAtPoint:CGPointZero];
                _target->_d->image = nil;
                function();
            }];
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
