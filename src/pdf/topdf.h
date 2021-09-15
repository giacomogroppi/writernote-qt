#ifndef TOPDF_H
#define TOPDF_H
#include "../touch/datastruct/datastruct.h"
#include "../mainwindow.h"

#include "../touch/tabletcanvas.h"

class topdf
{
private:
    TabletCanvas *m_canvas;

    Document *data;
    QString *path;

    void translate();

    MainWindow *parent;

    void draw(QPainter &painter, double m, int size_orizzontale, int size_verticale, double *y_last, const bool withPdf){
        bool isloading = true;
        QColor color = Qt::black;
        QPen m_pen;
        QBrush m_brush;
        struct TabletCanvas::Point lastPoint;

        m_canvas->load(painter,
                       this->data,
                       isloading,
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
                       y_last);
    }

    void updateBrush_load(float , QColor );

    /* variable to draw */
    struct Point {
        QPointF pos;
        qreal pressure = 0;
        qreal rotation = 0;
    } lastPoint;
    QPen penna;

public:
    ~topdf(){
        delete data;
    };

    topdf(QString *path, TabletCanvas *canvas);
    bool createpdf(const bool withPdf);

    inline void copy(Document *datastruct_){
            this->data = new Document;

            *this->data->datatouch = *datastruct_->datatouch;
    }
};

#endif // TOPDF_H
