#include "WPainter.h"
#include "core/Image/WImage.h"
#include "core/Image/WImagePrivate.h"

#import <CoreFoundation/CoreFoundation.h>
#import <UIKit/UIKit.h>

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

static auto createNSColor (const WColor &color) -> UIColor*
{
    return [UIColor
                colorWithRed:color.getRedNormalize()
                green:color.getGreenNormalize()
                blue:color.getBlueNormalize()
                alpha:color.getAlfaNormalize()];
}

WPainter::WPainter() noexcept
    : _target(nil)
    , _isAntialeasing(false)
    , _compositionMode(WPainter::CompositionMode::CompositionMode_SourceOver)
{
    
}

void WPainter::drawEllipse (const PointF &center, double rx, double ry)
{
    execute([this, &center, rx, ry]{
        WMutexLocker _(this->_lock);
        const double width = this->_pen.widthF();
        auto *color = createNSColor(this->_pen.color());
        
        // Impostare il colore e la dimensione dello stroke
        [color setStroke];
        UIBezierPath *path = [UIBezierPath bezierPathWithOvalInRect:CGRectMake(center.x() - rx / 2.,
                                                                               center.y() - ry / 2.,
                                                                               rx,
                                                                               ry)];
        path.lineWidth = width;
        
        [path stroke];
    });
}

void WPainter::drawImage (const RectF &target, const WImage &image, const RectF &source)
{
    if (image._d->image == nil) {
        WDebug(true, "Image is null.");
        return;
    }
    this->execute([this, &target, &image, &source](){
        WMutexLocker _(this->_lock);
        
        CGRect targetRect = CGRectMake(target.topLeft().x(), target.topLeft().y(), target.width(), target.height());
        CGRect cropRect = CGRectMake(source.topLeft().x(), source.topLeft().y(), source.width(), source.height());
        
        // Esegui il crop dell'immagine
        CGImageRef imageRef = CGImageCreateWithImageInRect([image._d->image CGImage], cropRect);
        UIImage *croppedImage = [UIImage imageWithCGImage:imageRef];
        CGImageRelease(imageRef);
        
        TIME_START(wpainter_drawimage);
        [croppedImage drawInRect:targetRect blendMode:kCGBlendModeNormal alpha:1.0];
        //TIME_STOP(wpainter_drawimage, "wpainter_drawimage");
    });
}

void WPainter::drawLine(int x1, int y1, int x2, int y2)
{
    execute([this, x1, y1, x2, y2]{
        WMutexLocker _(this->_lock);
        
        const double width = this->_pen.widthF();
        auto *color = createNSColor(_pen.color());
        
        [color setStroke];
        UIBezierPath *path = [UIBezierPath bezierPath];
        path.lineWidth = width;
        [path moveToPoint:CGPointMake(x1, y1)];
        [path addLineToPoint:CGPointMake(x2, y2)];
        [path stroke];
    });
    
    this->drawPoint(PointF(x1, y1));
    this->drawPoint(PointF(x2, y2));
}

void WPainter::drawPoint (const PointF &point)
{
    execute([this, &point]{
        WMutexLocker _(this->_lock);
        const double width = this->_pen.widthF() / 2.;
        auto *color = createNSColor(this->_pen.color());
        
        [color setStroke];
        UIBezierPath *path = [UIBezierPath bezierPathWithOvalInRect:CGRectMake(point.x() - width / 2.,
                                                                               point.y() - width / 2.,
                                                                               width,
                                                                               width)];
        path.lineWidth = width;
        
        [path fill];
        [path stroke];
    });
}

void WPainter::drawRect(const RectF &rectWriternote)
{
    execute([this, &rectWriternote]{
        WMutexLocker _(this->_lock);
        const double width = this->_pen.widthF();
        auto *color = createNSColor(_pen.color());

        const CGRect rect = CGRectMake(rectWriternote.topLeft().x(),
                                       rectWriternote.topLeft().y(),
                                       rectWriternote.width(),
                                       rectWriternote.height());

        [color setStroke];
        UIBezierPath *path = [UIBezierPath bezierPathWithRect:rect];
        path.lineWidth = width;

        [path stroke];
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
    this->_pen.setColor(color);
}

void WPainter::setPen(const WPen &pen)
{
    WMutexLocker _(this->_lock);
    this->_pen = pen;
}

void WPainter::setAntialiasing()
{
    WMutexLocker _(this->_lock);
    // TODO: adjust this to set this property always
    
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
    
    //auto realCompositionMode = getAdaptCompositionMode(_compositionMode);
    
    // TODO: adjust this to set this property always
    //CGContextSetBlendMode(UIGraphicsGetCurrentContext(), realCompositionMode);
}

WPainter::~WPainter()
{
    W_ASSERT(!isActive());
}

auto WPainter::height() const -> int
{
    W_ASSERT(_target != nullptr);
    return this->_target->rect().height();
}

auto WPainter::width() const -> int
{
    W_ASSERT(_target != nullptr);
    return this->_target->rect().width();
}
