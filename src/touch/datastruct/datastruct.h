#ifndef DATASTRUCT_H
#define DATASTRUCT_H

#include <QList>
#include <QColor>
#include <QImage>

#include "../../images/image_struct.h"

struct colore_s{
    int colore[4];
};

class datastruct
{
public:
    datastruct();
    void reset();
    void stampa();
    int last();

    int firsty();
    int firstx();

    double biggerynoid();
    double biggerx();
    void removeat(int i);

    bool needtocreatenew();
    bool needtochangeid(int);

    double biggery();

    /* la funzione viene richiamata quando viene compresso il file in un file zip -> false se ci sono dei negativi < 0 */
    bool ispositivey();
    bool ispositivex();

    QList<double> x;
    QList<double> y;
    QList<int> posizioneaudio;
    QList<struct colore_s> color;

    QList<float> pressure;
    QList<qreal> rotation;

    QList<struct immagine_S> immagini = {};

    QList<int> idtratto;

    QList<int> posizionefoglio;
    /* il valore viene settare da 0 a 100 in base a quanto zoom c'è
    2 -> massimo zoom
    0 -> minimo zoom
    */
    float zoom = 1.00;
};

#endif // DATASTRUCT_H
