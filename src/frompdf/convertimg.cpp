#include "convertImg.h"
#include <QDebug>

void convertImg::run(){
    qDebug() << "Start";
    *img = page->renderToImage(precision, precision);
    qDebug() << "Finish";
}
