#include "WPixmap.h"

#include "touch/dataTouch/page/Page.h"
#include <QPixmap>

WPixmap::WPixmap(int page, bool consideringResolution)
    : QPixmap(consideringResolution
                 ? Page::getResolutionWidth()
                 : Page::getWidth(),
             static_cast<int>(consideringResolution
                                  ? Page::getResolutionHeigth()
                                  : Page::getHeight()
                              ) * page)
{

}
