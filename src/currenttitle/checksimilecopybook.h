#ifndef CHECKSIMILECOPYBOOK_H
#define CHECKSIMILECOPYBOOK_H

#define OK 0
#define COLORE 1
#define XCHECK 2
#define YCHECK 3
#define ROTATION 3
#define PRESSURE 4
#define IDTRATTO 5

#define TESTINOHTML 6
#define TESTI 7
#define AUDIOPOSITION 8

#include "currenttitle_class.h"
#include "../indice_class.h"

int checksimilecopybook(currenttitle_class *, currenttitle_class *);
bool checksimileindice(indice_class *, indice_class *);

#endif // CHECKSIMILECOPYBOOK_H
