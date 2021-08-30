#include "convertImg.h"

void convertImg::run(){
    *img = page->renderToImage(precision, precision);
}
