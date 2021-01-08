#ifndef TOPDF_H
#define TOPDF_H
#include "../touch/datastruct/datastruct.h"
#include "../mainwindow.h"


class topdf
{
private:
    datastruct *data;
    QString *path;

    void translate();

    MainWindow *parent;

    void draw( QPainter *, double );
    void updateBrush_load(float , QColor );

    /* variable to draw */
    struct Point {
        QPointF pos;
        qreal pressure = 0;
        qreal rotation = 0;
    } lastPoint;
    QPen penna;



public:


    topdf(QString *path);
    bool createpdf();

    inline void copy(datastruct *datastruct_){
            this->data = new datastruct;

            this->data->x = datastruct_->x;
            this->data->y = datastruct_->y;
            this->data->color = datastruct_->color;
            this->data->idtratto = datastruct_->idtratto;
            this->data->posizioneaudio = datastruct_->posizioneaudio;
            this->data->posizionefoglio = datastruct_->posizionefoglio;
            this->data->pressure = datastruct_->pressure;
            this->data->rotation = datastruct_->rotation;
            this->data->zoom = datastruct_->zoom;
    }
};

#endif // TOPDF_H
