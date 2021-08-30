#ifndef CONVERTIMG_H
#define CONVERTIMG_H

#include <QObject>
#include <QThread>
#include "poppler-qt5.h"
#include <QImage>

class convertImg: public QThread{
Q_OBJECT
public:
    convertImg(const Poppler::Page *page, QImage *img, const uint precision);
    ~convertImg();
protected:
    void run();
private:
    const Poppler::Page *page;
    QImage *img;
    uint precision;
};

inline convertImg::convertImg(const Poppler::Page *page, QImage *img, const uint precision)
{
    this->img = img;
    this->page = page;
    this->precision = precision;
}

inline convertImg::~convertImg()
{
    delete page;
}

#endif // CONVERTIMG_H
