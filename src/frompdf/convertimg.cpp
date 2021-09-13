#include "convertImg.h"
#include <QDebug>
#ifdef PDFSUPPORT

void convertImg::run(){
    qDebug() << "Start";
    *img = page->renderToImage(precision, precision);
    qDebug() << "Finish";
}

#endif