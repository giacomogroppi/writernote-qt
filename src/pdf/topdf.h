#ifndef TOPDF_H
#define TOPDF_H
#include "../touch/datastruct/datastruct.h"
#include "../mainwindow.h"

#include "../touch/tabletcanvas.h"

class topdf
{
private:
    TabletCanvas *m_canvas;

    currenttitle_class *data;
    QString *path;

    void translate();

    MainWindow *parent;

    void draw(QPainter &, double , int , int, double *y_last);
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
    bool createpdf();

    inline void copy(currenttitle_class *datastruct_){
            this->data = new currenttitle_class;

            *this->data->datatouch = *datastruct_->datatouch;
    }
};

#endif // TOPDF_H
