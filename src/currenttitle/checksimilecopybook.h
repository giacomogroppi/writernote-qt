#ifndef CHECKSIMILECOPYBOOK_H
#define CHECKSIMILECOPYBOOK_H

#define OK_CHECK 0
#define COLORE 1
#define XCHECK 2
#define YCHECK 3
#define ROTATION 4
#define PRESSURE 5
#define IDTRATTO 6

#define TESTINOHTML 7
#define TESTI 8
#define AUDIOPOSITION 9

/*
 * for the new data struct
*/
#define LEN 10

#include "currenttitle_class.h"
#include "../indice_class.h"

int checksimilecopybook(currenttitle_class *, currenttitle_class *);
bool checksimileindice(indice_class *, indice_class *);

#endif // CHECKSIMILECOPYBOOK_H
