#pragma once

#include "touch/dataTouch/datastruct/DataStruct.h"
#include "touch/TabletUtils.h"

class topdf
{
private:
    class Document *data = nullptr;
    const WString *path;

    void translate();

    void draw(WPainter &painter, Double m,
              Bool withPdf);

public:
    topdf(const WString &path, const Document &doc);
    ~topdf();

    bool createpdf(cbool withPdf);

};

inline void topdf::draw(WPainter &painter, Double m, Bool withPdf)
{
    TabletUtils loader(painter,
        Bool(false),
        Unsigned(0u),
        m,
        Optional<Laser>(),
        *data,
        withPdf,
        Bool(true),
        RectF()
    );

    loader.load(UpdateEvent::stroke | UpdateEvent::page | UpdateEvent::sheet);
}

