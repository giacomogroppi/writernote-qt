#ifndef TOPDF_H
#define TOPDF_H
#include "../touch/datastruct/datastruct.h"
#include "../mainwindow.h"
#include "../touch/tabletcanvas.h"

class topdf
{
private:
    Document *data = nullptr;
    const QString *path;

    void translate();

    MainWindow *parent;

    void draw(QPainter &painter, double m,
              int size_orizzontale, int size_verticale, const bool withPdf){

        QColor color = Qt::black;
        QPen m_pen;
        QBrush m_brush;
        Point lastPoint;

        TabletCanvas::load(painter,
                           this->data,
                           color,
                           m_pen,
                           m_brush,
                           lastPoint,
                           -1,
                           nullptr,
                           withPdf,
                           m,
                           size_orizzontale,
                           size_verticale,
                           nullptr,
                           false);
    }

    void updateBrush_load(const double , QColor );

    QPen penna;

public:
    ~topdf(){
        delete data;
    };

    topdf(const QString &path);
    bool createpdf(const bool withPdf);

    inline void copy(Document *datastruct_){
        this->data = new Document;
        Document::copy(*datastruct_, *data);
    }
};

#endif // TOPDF_H
