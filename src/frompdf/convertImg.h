#pragma once

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

#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
    void setData(const Poppler::Page *page, QImage *img);
#else
    void setData(const Poppler::Document *doc, QImage *img, int index);
#endif

protected:
    void run();

private:
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
    const Poppler::Page *page = NULL;
#else
    const Poppler::Document *doc = NULL;
    int index = -1;
#endif
    QImage *img = NULL;
    uint precision;
};

#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
inline void convertImg::setData(const Poppler::Page *page, QImage *img)
{
    this->img = img;
    this->page = page;
}
#else
inline void convertImg::setData(const Poppler::Document *doc, QImage *img, int index)
{
    this->doc = doc;
    this->img = img;
    this->index = index;
}
#endif

#endif // PDFSUPPORT
