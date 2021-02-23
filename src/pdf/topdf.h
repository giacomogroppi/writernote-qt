#ifndef TOPDF_H
#define TOPDF_H
#include "../touch/datastruct/datastruct.h"
#include "../mainwindow.h"


class topdf
{
private:
    currenttitle_class *data;
    QString *path;

    void translate();

    MainWindow *parent;

    void draw( QPainter *, double , int , int);
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

    topdf(QString *path);
    bool createpdf();

    inline void copy(currenttitle_class *datastruct_){
            this->data = new currenttitle_class;

            this->data->m_touch = datastruct_->m_touch;

            /*
             * last data struct
            */
            /*data->datatouch->x = datastruct_->datatouch->x;
            data->datatouch->y = datastruct_->datatouch->y;
            data->datatouch->color = datastruct_->datatouch->color;
            data->datatouch->idtratto = datastruct_->datatouch->idtratto;
            data->datatouch->posizioneaudio = datastruct_->datatouch->posizioneaudio;
            data->datatouch->posizionefoglio = datastruct_->datatouch->posizionefoglio;
            data->datatouch->pressure = datastruct_->datatouch->pressure;
            data->datatouch->rotation = datastruct_->datatouch->rotation;
            data->datatouch->zoom = datastruct_->datatouch->zoom;*/
    }
};

#endif // TOPDF_H
