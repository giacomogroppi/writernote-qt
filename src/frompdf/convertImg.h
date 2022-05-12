#ifndef CONVERTIMG_H
#define CONVERTIMG_H

#ifdef PDFSUPPORT

#include <QObject>
#include <QThread>

#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
# include "poppler-qt5.h"
#else
# include "poppler-qt6.h"
#endif

#include <QImage>

class convertImg: public QThread{
    //Q_OBJECT
public:
    convertImg(const uint precision);
    ~convertImg();

    void setData(const Poppler::Page *page, QImage *img);

protected:
    void run();

private:
    const Poppler::Page *page = NULL;
    QImage *img = NULL;
    uint precision;
};

inline void convertImg::setData(const Poppler::Page *page, QImage *img)
{
    this->img = img;
    this->page = page;
}

#endif
#endif // CONVERTIMG_H
