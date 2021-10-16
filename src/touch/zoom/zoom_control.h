#ifndef ZOOM_CONTROL_H
#define ZOOM_CONTROL_H

class frompdf;
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
public:
    bool zoom(QPointF &,
              double,
              const uint widthP,
              const uint maxWidth,
              const uint height,
              const uint maxHeight,
              datastruct *data);

};

#endif // ZOOM_CONTROL_H
