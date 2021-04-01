#ifndef ZOOM_CONTROL_H
#define ZOOM_CONTROL_H

class datastruct;
#include <QPointF>

class zoom_control
{
private:
    /*
     *  la funzione prense il punto passatogli
     *  e sposta il punto in (1,1) per facilitare i conti
     *  -> dopo risposta il tutto
     *
     *  true -> inverse operation
    */
    void trasla(QPointF ,
                datastruct *data,
                long double delta = 0);
    bool needToResizeWidth(int widthP,
                           int maxWidth,
                           datastruct *data);
public:
    zoom_control();

    bool zoom(QPointF &,
              long double,
              const unsigned int widthP,
              const unsigned int maxWidth,
              const unsigned int height,
              const unsigned int maxHeight,
              datastruct *data);

};

#endif // ZOOM_CONTROL_H
