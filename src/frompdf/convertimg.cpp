#include "convertImg.h"
#include <QDebug>
#ifdef PDFSUPPORT

void convertImg::run(){
    *img = page->renderToImage(precision, precision);
}

#endif
