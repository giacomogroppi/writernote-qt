#include "WImage.h"
#import <CoreFoundation/CoreFoundation.h>
#import <UIKit/UIKit.h>
#import <CoreGraphics/CoreGraphics.h>

/*
#include "touch/dataTouch/page/Page.h"

#ifdef Q_OS_IOS
WImage::WImage(int page, bool consideringResolution)
{
  CGSize imageSize = CGSizeMake(
    consideringResolution
      ? Page::getResolutionWidth()
      : Page::getWidth(),
    consideringResolution
      ? Page::getResolutionHeigth() * page
      : Page::getHeight());

  UIGraphicsImageRenderer *renderer = [[UIGraphicsImageRenderer alloc] initWithSize:imageSize opaque:NO];

  this->d = [renderer imageWithActions:^(UIGraphicsImageRendererContext * _Nonnull rendererContext) {
      // non viene disegnato nulla qui, l'immagine ha solo lo sfondo trasparente
  }];
}



QImage WImage::toQImage() const
{
    NSData *imageData = UIImagePNGRepresentation((UIImage *)this->d);

    // Creare un oggetto QBuffer e scrivere i dati dell'immagine
    QBuffer buffer;
    buffer.setData((const char *)imageData.bytes, imageData.length);
    buffer.open(QIODevice::ReadOnly);

    // Creare un oggetto QImage a partire dai dati dell'immagine
    QImage qImage;
    qImage.load(&buffer, "PNG");

    return qImage;
}
#endif
*/
