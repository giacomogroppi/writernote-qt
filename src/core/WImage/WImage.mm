#import <CoreFoundation/CoreFoundation.h>
#import <CoreGraphics/CoreGraphics.h>
#import <Cocoa/Cocoa.h>
#import <Foundation/Foundation.h>
#import "WImage.h"
#include "touch/DataTouch/Page/Page.h"

#define getData(data) (__bridge NSImage *) data

WImage::WImage () noexcept
    : d(nullptr)
{
    d = (__bridge void *)[[NSImage alloc] init];
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
    other.d = (__bridge void *)[[NSImage alloc] init];
}

WImage::WImage (const WImage &other) noexcept
    : d(nullptr)
{
    NSImage *originalImage = (__bridge NSImage *)other.d;

    // Create a copy of the original NSImage
    NSImage *copiedImage = [[NSImage alloc] initWithSize:originalImage.size];
    [copiedImage lockFocus];
    [originalImage drawAtPoint:NSZeroPoint fromRect:NSZeroRect operation:NSCompositingOperationCopy fraction:1.0];
    [copiedImage unlockFocus];
}
