#ifndef CHECKSIMILECOPYBOOK_H
#define CHECKSIMILECOPYBOOK_H

#define OK_CHECK 0
#define COLORE 1
#define XCHECK 2
#define YCHECK 3
#define ROTATION 4
#define PRESSURE 5
#define IDTRATTO 6
#define POSIZIONE_FOGLIO 7

#define TESTINOHTML 8
#define TESTI 9
#define AUDIOPOSITION 10

#define LEN_POSIZIONEFOGLIO 11

/*
 * for the new data struct
*/
#define LEN 10

#include "currenttitle_class.h"
#include "../indice_class.h"

int checksimilecopybook(currenttitle_class *, currenttitle_class *, bool );
int checksimileindice(indice_class *, indice_class *);

#endif // CHECKSIMILECOPYBOOK_H
