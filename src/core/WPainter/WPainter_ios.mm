#include "WPainter.h"
#include "core/Image/WImage.h"
#include "core/Image/WImagePrivate.h"

#import <CoreFoundation/CoreFoundation.h>
//#import <Cocoa/Cocoa.h>
#import <UIKit/UIKit.h>

class WPainterPrivate {
public:
    
};

static auto getAdaptCompositionMode (WPainter::CompositionMode compositionMode) -> CGBlendMode
{
    switch (compositionMode)
    {
        case WPainter::CompositionMode::CompositionMode_Clear:
            return CGBlendMode::kCGBlendModeClear;
        case WPainter::CompositionMode::CompositionMode_SourceOver:
            return CGBlendMode::kCGBlendModeDestinationIn;
        case WPainter::CompositionMode::CompositionMode_DestinationOver:
            return CGBlendMode::kCGBlendModeDestinationOver;
    }
    W_ASSERT_TEXT(0, "Composition mode not implemented");
}

static auto createNSColor (const WColor &color) -> UIColor*
{
    return [UIColor
                colorWithRed:color.getRedNormalize()
                green:color.getGreenNormalize()
                blue:color.getBlueNormalize()
                alpha:color.getAlfaNormalize()];
}

static auto executeOnMainThread (dispatch_block_t method, int w, int h, WImage *target) -> UIImage *
{
    __block UIImage *result = nil;
    
    dispatch_block_t realMethod = ^{
        UIGraphicsBeginImageContext(CGSizeMake(w, h));
        
        UIImage *t = target->_d->image;
        [t drawAtPoint:CGPointZero];
        
        method();
        
        result = UIGraphicsGetImageFromCurrentImageContext();
        result = [[UIImage alloc] init];
        UIGraphicsEndImageContext();
    };
    
    if (![NSThread isMainThread])
        dispatch_sync(dispatch_get_main_queue(), realMethod);
    else
        realMethod();
    
    W_ASSERT(result != nil);
    
    return result;
}

WPainter::WPainter() noexcept
    : _target(nil)
    , _isAntialeasing(false)
    , _compositionMode(WPainter::CompositionMode::CompositionMode_SourceOver)
    , _color(color_black)
{
    
}

void WPainter::drawEllipse (const PointF &center, double rx, double ry)
{
    _target->_d->image = executeOnMainThread(^{
        WMutexLocker _(this->_lock);
        const auto *image = _target->_d->image;
        const double width = this->_pen.widthF();
        auto *color = createNSColor(this->_color);

        [image drawAtPoint:CGPointZero];
        
        // Impostare il colore e la dimensione dello stroke
        [color setStroke];
        UIBezierPath *path = [UIBezierPath bezierPathWithOvalInRect:CGRectMake(center.x() - rx / 2.,
                                                                               center.y() - ry / 2.,
                                                                               center.x() + rx / 2.,
                                                                               center.y() + ry / 2.)];
        path.lineWidth = width;
        
        [path stroke];
    }, width(), height(), _target);
}

void WPainter::drawImage (const RectF &target, const WImage &image, const RectF &source)
{
    _target->_d->image = executeOnMainThread(^{
        WMutexLocker _(this->_lock);
        auto *imageSource = (UIImage *)image._d->image;
        
        CGRect targetRect = CGRectMake(target.topLeft().x(), target.topLeft().y(), target.bottomRight().x(), target.bottomRight().y());
        CGRect sourceRect = CGRectMake(source.topLeft().x(), source.topLeft().y(), source.bottomRight().x(), source.bottomRight().y());

        [imageSource drawInRect:sourceRect];
    }, width(), height(), _target);
}

void WPainter::drawLine(int x1, int y1, int x2, int y2)
{
    _target->_d->image = executeOnMainThread(^{
        WMutexLocker _(this->_lock);
        
        const double width = this->_pen.widthF();
        auto *color = createNSColor(this->_color);
        
        [color setStroke];
        UIBezierPath *path = [UIBezierPath bezierPath];
        path.lineWidth = width;
        [path moveToPoint:CGPointMake(x1, y1)];
        [path addLineToPoint:CGPointMake(x2, y2)];
        [path stroke];
    }, width(), height(), _target);
}

void WPainter::drawPoint (const PointF &point)
{
    return this->drawRect(RectF {
        point.x() - 1., point.y() - 1.,
        point.x() + 1., point.y() + 1.
    });
}

void WPainter::drawRect(const RectF &rectWriternote)
{
    _target->_d->image = executeOnMainThread(^{
        WMutexLocker _(this->_lock);
        const double width = this->_pen.widthF();
        auto *color = createNSColor(this->_color);

        const CGRect rect = CGRectMake(rectWriternote.topLeft().x(),
                                       rectWriternote.topLeft().y(),
                                       rectWriternote.bottomRight().x(),
                                       rectWriternote.bottomRight().y());

        [color setStroke];
        UIBezierPath *path = [UIBezierPath bezierPathWithOvalInRect:rect];
        path.lineWidth = width;

        [path stroke];
    }, width(), height(), _target);
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
    // TODO: adjust this to set this property always
    CGContextSetAllowsAntialiasing(UIGraphicsGetCurrentContext(), YES);
    CGContextSetShouldAntialias(UIGraphicsGetCurrentContext(), YES);
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
    
    auto realCompositionMode = getAdaptCompositionMode(_compositionMode);
    
    // TODO: adjust this to set this property always
    CGContextSetBlendMode(UIGraphicsGetCurrentContext(), realCompositionMode);
}

bool WPainter::begin(WImage *pixmap)
{
    WMutexLocker _(this->_lock);
    W_ASSERT_TEXT(pixmap != nullptr, "Pixmap passed as target is null");
    W_ASSERT_TEXT(!isActive(), "Trying to begin on an WPainter not ended");
    
    this->_target = pixmap;

    return true;
}

auto WPainter::end() -> bool
{
    _target = nullptr;
    
    return true;
}

WPainter::~WPainter()
{
    const auto wasActive = this->isActive();
    if (wasActive)
        this->end();
    WDebug(wasActive, "Painter was active...");
}

auto WPainter::height() const -> int
{
    return this->_target->rect().height();
}

auto WPainter::width() const -> int
{
    return this->_target->rect().width();
}
