#include "WPainter.h"

/*
#import <CoreFoundation/CoreFoundation.h>
#import <UIKit/UIKit.h>
#import <CoreGraphics/CoreGraphics.h>


WPainter::WPainter(WImage *img)
  : _img(img)
{
  CGSize imageSize = CGSizeMake(100, 100);
  UIGraphicsImageRenderer *renderer = [[UIGraphicsImageRenderer alloc] initWithSize:imageSize];
}

void WPainter::drawLine(const QPointF &p1, const QPointF &p2)
{
  const double x1 = p1.x();
  const double y1 = p1.y();
  const double x2 = p2.x();
  const double y2 = p2.y();

  UIGraphicsBeginImageContextWithOptions(((UIImage *)_img->d).size, NO, 0);

  [(UIImage *)_img->d drawAtPoint:CGPointZero];

  CGContextRef context = UIGraphicsGetCurrentContext();
  CGContextSetLineWidth(context, 5.0);
  CGContextSetStrokeColorWithColor(context, [UIColor colorWithRed:this->color.getRed() / 255.0f
                                                     green:this->color.getGreen() / 255.0f
                                                     blue:this->color.getBlue() / 255.0f
                                                     alpha:this->color.getAlfa() / 255.0f].CGColor);
  CGContextMoveToPoint(context, x1, y1);
  CGContextAddLineToPoint(context, x2, y2);
  CGContextStrokePath(context);

  _img->d = UIGraphicsGetImageFromCurrentImageContext();

  UIGraphicsEndImageContext();
}

*/
