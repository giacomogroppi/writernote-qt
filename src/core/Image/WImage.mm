#import <CoreFoundation/CoreFoundation.h>
#import <CoreGraphics/CoreGraphics.h>
#import <Cocoa/Cocoa.h>
#import <Foundation/Foundation.h>
#import "WImage.h"
#include "touch/DataTouch/Page/Page.h"

#define getData(data) (__bridge NSImage *) data

struct WImagePrivate {
    NSImage *image;
};

WImage::WImage () noexcept
    : d(nullptr)
{
    //d = (__bridge void *)[[NSImage alloc] init];
}

WImage::WImage (int page, bool consideringResolution)
    : d(nullptr)
{
    const NSInteger width  = consideringResolution ? Page::getResolutionWidth() : Page::getWidth();
    const NSInteger height = static_cast<int>(
                                consideringResolution ? Page::getResolutionHeigth() : Page::getHeight()
                             ) * page;
    d = (__bridge void *)[[NSImage alloc] initWithSize:NSMakeSize(width, height)];
}

WImage::WImage (int width, int height, WImageType format)
    : d(nullptr)
{
    d = (__bridge void *)[[NSImage alloc] initWithSize:NSMakeSize(width, height)];
}

WImage::WImage (WImage &&other) noexcept
    : d(other.d)
{
    other.d = nullptr;
}

WImage::WImage (const WImage &other) noexcept
    : d(nullptr)
{
    NSImage *originalImage = (__bridge NSImage *)other.d;
    NSImage *imageCopy = [originalImage mutableCopy];
    this->d = (__bridge void *)imageCopy;
}

auto WImage::loadFromData(const WByteArray &data, const char *formact) -> bool
{
    W_ASSERT(strcmp(formact, "PNG") == 0);
    // Assume you have the PNG data in an NSData object named "imageData"
    NSData *imageData = [NSData dataWithBytes:data.constData() length:data.size()];

    // Crea un oggetto NSImage utilizzando i dati PNG
    this->d = (__bridge void *)[[NSImage alloc] initWithData:imageData];

    return true;
}

auto WImage::save_and_size(WByteArray &arr) const -> size_t
{
    // Assume you have an existing NSImage object named "image"
    NSImage *image = getData(d);

    // Crea un oggetto NSBitmapImageRep utilizzando il TIFFRepresentation dell'immagine
    NSBitmapImageRep *bitmapRep = [NSBitmapImageRep imageRepWithData:[image TIFFRepresentation]];

    // Crea un buffer dati per contenere l'immagine codificata in PNG
    NSMutableData *imageData = [NSMutableData data];

    // Codifica l'immagine in PNG e aggiungi i dati al buffer
    NSData *pngData = [bitmapRep representationUsingType:NSPNGFileType properties:@{}];
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
    const NSImage *my = getData(d);
    const NSImage *ot = getData(other.d);
    return [my isEqualTo:ot];
}

auto WImage::operator=(WImage &&other) noexcept -> WImage &
{
    this->d = other.d;
    other.d = nullptr;
    return *this;
}

auto WImage::operator=(const WImage &other) noexcept -> WImage &
{
    NSImage *copy = [[NSImage alloc] init];
    d = (__bridge void *)[copy mutableCopy];
    return *this;
}

WImage::WImage(const WString &path) noexcept
    : d(nullptr)
{
    const char *position = path.toUtf8().constData();
    NSString *s = [NSString stringWithUTF8String:position];
    NSURL *url = [[NSURL alloc] initWithString:s];
    
    NSImage *image = [[NSImage alloc] initWithContentsOfURL:url];
    d = (__bridge void *) image;
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
    NSImage *img = getData(d);
    NSSize size = img.size;
    return RectF {
        0., 0.,
        size.width,
        size.height
    };
}

auto WImage::isNull() const -> bool
{
    return this->d != nullptr;
}

auto WImage::fill(const WColor &color) -> void
{
    NSImage *image = getData(d);
    const auto rect = this->rect();
    
    [image lockFocus];

    NSColor *rectangleColor = [NSColor colorWithCalibratedRed:color.getRed() green:color.getGreen() blue:color.getBlue() alpha:color.getAlfa()];

    [rectangleColor set];
    NSRectFill(NSMakeRect(0, 0, rect.width(), rect.height()));

    [image unlockFocus];
}

auto WImage::pixel(const WPoint &point) const -> WRgb
{
    NSImage *image = getData(d);
    NSBitmapImageRep* raw_img = [NSBitmapImageRep imageRepWithData:[image TIFFRepresentation]];
    NSColor* color = [raw_img colorAtX:point.x() y:point.y()];
    
    const char blue = color.blueComponent * 255;
    const char green = color.greenComponent * 255;
    const char red = color.redComponent * 255;
    const char alfa = color.alphaComponent * 255;
    
    return red | green << 8 | blue << 16 | alfa << 24;
}
