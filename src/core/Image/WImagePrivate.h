#pragma once

#import <CoreFoundation/CoreFoundation.h>
#import <CoreGraphics/CoreGraphics.h>
#import <UIKit/UIKit.h>
#import <Foundation/Foundation.h>

struct WImagePrivate {
    UIImage *image;
    WImagePrivate() : image(nullptr) {}
};
