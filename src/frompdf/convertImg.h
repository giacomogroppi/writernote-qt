#ifndef CONVERTIMG_H
#define CONVERTIMG_H

#ifdef PDFSUPPORT

#include <QObject>
#include <QThread>
#include "poppler-qt5.h"
#include <QImage>

class convertImg: public QThread{
    Q_OBJECT
public:
    convertImg(const uint precision);
    ~convertImg();

    void setData(const Poppler::Page *page, QImage *img);

protected:
    void run();

private:
    const Poppler::Page *page;
    QImage *img;
    uint precision;
};

inline void convertImg::setData(const Poppler::Page *page, QImage *img)
{
    this->img = img;
    this->page = page;
}

#endif
#endif // CONVERTIMG_H
