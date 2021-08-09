#ifndef FROMIMAGE_H
#define FROMIMAGE_H

#include "zip.h"
#include <QList>
#include "../utils/common_error_definition.h"
#include <QPointF>
#include <QImage>

class Document;
struct immagine_S{
    QImage immagini;
    QPoint i;
    QPoint f;
};

class fromimage{
private:
    Document *doc;

    QList<struct immagine_S> m_img;
    inline QStringList get_name_img();

    static inline QString getName(const QString &p, const uint i){
        return p + "_img_" + QString::number(uint(i));
    }

public:
    inline fromimage(Document *doc){
        this->doc = doc;
    }

    uchar load(zip_t *file);
    uchar save(zip_t *file) const;

    inline void move(const QPointF &translation){

    }

    inline void draw(QPainter &painter,
                     const uint pwidth,
                     const int rend_width,
                     const int rend_heigth){

    }

    void reset(){
        m_img.clear();
    }
};

#endif // FROMIMAGE_H
