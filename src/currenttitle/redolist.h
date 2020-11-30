#ifndef REDOLIST_H
#define REDOLIST_H

#include "../mainwindow.h"
/*
Siccome con un singolo click cambia l'item nella lista, è necessario, in caso non si
voglia passare a un altro copybook, reimpostare quello precedente.
*/
void redolist(MainWindow *parent);


#endif // REDOLIST_H
