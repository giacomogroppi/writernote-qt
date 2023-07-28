#pragma once

#import <CoreFoundation/CoreFoundation.h>
#import <CoreGraphics/CoreGraphics.h>
#import <Cocoa/Cocoa.h>
#import <Foundation/Foundation.h>

struct WImagePrivate {
    NSImage *image;
    WImagePrivate() : image(nullptr) {}
};
