#import <CoreFoundation/CoreFoundation.h>
#import <CoreGraphics/CoreGraphics.h>
#import <Foundation/Foundation.h>
#import "WImage.h"
#include "touch/DataTouch/Page/Page.h"
#include "WImagePrivate.h"
#import <UIKit/UIKit.h>

WImage::WImage () noexcept
    : _d(new WImagePrivate)
    , _rect()
{
    //d = (__bridge void *)[[NSImage alloc] init];
    _d->_renderer = [UIGraphicsImageRenderer alloc];
}

WImage::WImage (int page, bool consideringResolution)
    : _d(new WImagePrivate)
{
    const NSInteger width  = consideringResolution ? Page::getResolutionWidth() : Page::getWidth();
    const NSInteger height = static_cast<int>(
                                              consideringResolution ? Page::getResolutionHeigth() : Page::getHeight()
                                              ) * page;
    
    _d->image = [[UIImage alloc] init];
    _d->_renderer = [[UIGraphicsImageRenderer alloc] initWithSize:CGSizeMake(width, height)];
    _rect = RectF(0, 0, width, height);
}

WImage::WImage (int width, int height, WImageType format)
    : _d(new WImagePrivate)
    , _rect(0, 0, width, height)
{
    _d->image = [[UIImage alloc] init];
    _d->_renderer = [[UIGraphicsImageRenderer alloc] initWithSize:CGSizeMake(width, height)];
}

WImage::WImage (WImage &&other) noexcept
    : _d(std::move(other._d))
    , _rect(other._rect)
{
}

WImage::WImage (const WImage &other) noexcept
    : _d(new WImagePrivate)
    , _rect(other._rect)
{
    UIImage *originalImage = other._d->image;
    UIImage *imageCopy = [originalImage mutableCopy];
    _d->image = imageCopy;
}

auto WImage::loadFromData(const WByteArray &data, const char *formact) -> bool
{
    W_ASSERT(strcmp(formact, "PNG") == 0);
    // Assume you have the PNG data in an NSData object named "imageData"
    NSData *imageData = [NSData dataWithBytes:data.constData() length:data.size()];

    // Crea un oggetto NSImage utilizzando i dati PNG
    _d->image = [[UIImage alloc] initWithData:imageData];

    // TODO: size of image
    
    return true;
}

auto WImage::save_and_size(WByteArray &arr) const -> size_t
{
    // Assume you have an existing NSImage object named "image"
    UIImage *image = _d->image;

    // Crea un buffer dati per contenere l'immagine codificata in PNG
    NSMutableData *imageData = [NSMutableData data];

    // Codifica l'immagine in PNG e aggiungi i dati al buffer
    NSData *pngData = UIImagePNGRepresentation((UIImage *) image);
    [imageData appendData:pngData];
    arr = WByteArray (
                      static_cast<const char *>(imageData.bytes),
                      static_cast<unsigned long>(imageData.length)
                      );
    return arr.size();
}

auto WImage::operator==(const WImage &other) const -> bool
{
    if (this == &other)
        return true;
    const UIImage *my = _d->image;
    const UIImage *ot = other._d->image;

    NSData *d1 = UIImagePNGRepresentation((UIImage *) my);
    NSData *d2 = UIImagePNGRepresentation((UIImage *) ot);
    
    W_ASSERT([d1 isEqual: d2] == [my isEqual:ot]);
    
    return [my isEqual:ot];
}

auto WImage::operator=(WImage &&other) noexcept -> WImage &
{
    this->_d = std::move(other._d);
    this->_rect = other._rect;
    return *this;
}

auto WImage::operator=(const WImage &other) noexcept -> WImage &
{
    UIImage *copy = [[UIImage alloc] init];
    _d->image = [copy mutableCopy];
    return *this;
}

WImage::WImage(const WString &path) noexcept
    : _d(new WImagePrivate)
{
    const char *position = path.toUtf8().constData();
    NSString *s = [NSString stringWithUTF8String:position];
    
    _d->image = [UIImage imageWithContentsOfFile:s];
}

auto WImage::toImage() const -> WImage
{
    return WImage(*this);
}

auto WImage::getRawDataPNG() const -> WByteArray
{
    WByteArray result;
    (void) this->save_and_size(result);
    return result;
}

auto WImage::rect() const -> RectF
{
    return this->_rect;
    UIImage *img = _d->image;
    CGSize size = img.size;
    return RectF {
        0., 0.,
        size.width,
        size.height
    };
}

auto WImage::isNull() const -> bool
{
    return _d->image;
}

auto WImage::fill(const WColor &color) -> void
{
    const auto rect = this->rect();

    //NSColor *color = [NSColor colorWithCalibrateRed:color.getRed() green:color.getGreen() blue:color.getBlue() alpha:color.getAlfa()];
    [[UIColor colorWithRed:color.getRedNormalize() green:color.getGreenNormalize() blue:color.getBlueNormalize() alpha:color.getAlfaNormalize()] setFill];
    
    UIRectFill (CGRectMake(rect.topLeft().x(), rect.topLeft().y(), rect.bottomRight().x(), rect.bottomRight().y()));
    
    _d->image = UIGraphicsGetImageFromCurrentImageContext();
}

auto WImage::pixel(const WPoint &point) const -> WRgb
{
    UIImage *image = _d->image;
    
    CGImageRef imageRef = [image CGImage];
    CGDataProviderRef provider = CGImageGetDataProvider(imageRef);
    CFDataRef data = CGDataProviderCopyData(provider);
    const UInt8* bytes = CFDataGetBytePtr(data);

    size_t bytesPerRow = CGImageGetBytesPerRow(imageRef);
    size_t bitsPerComponent = CGImageGetBitsPerComponent(imageRef);
    size_t componentsPerPixel = CGImageGetBitsPerPixel(imageRef) / bitsPerComponent;
        
    size_t pixelIndex = ((size_t)point.y() * bytesPerRow) + ((size_t)point.x() * componentsPerPixel);

    UInt8 red = bytes[pixelIndex];
    UInt8 green = bytes[pixelIndex + 1];
    UInt8 blue = bytes[pixelIndex + 2];
    UInt8 alpha = bytes[pixelIndex + 3];

    CFRelease(data);
    
    return red | green << 8 | blue << 16 | alpha << 24;
}
