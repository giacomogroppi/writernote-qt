#pragma once

#ifdef PDFSUPPORT

#include "Scheduler/WObject.h"

#include "poppler-qt6.h"

#include "core/WImage.h"

class convertImg: public QThread{
    //Q_OBJECT
public:
    convertImg(int precision);
    ~convertImg();

    void setData(const Poppler::Document *doc, WImage *img, int index);

protected:
    void run();

private:
    const Poppler::Document *doc = NULL;
    int index = -1;

    WImage *img = NULL;
    uint precision;
};

inline void convertImg::setData(const Poppler::Document *doc, WImage *img, int index)
{
    this->doc = doc;
    this->img = img;
    this->index = index;
}

#endif // PDFSUPPORT
