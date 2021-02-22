#include "mostra_explorer.h"

#include <QString>
#include <QFuture>
#include <QtConcurrent/QtConcurrent>

static void mostra(const char *comando);

void mostra_explorer(const char *posizione)
{
    const char *comando;

#ifdef unix
    comando = ((QString)"nautilus " + posizione).toUtf8().constData();

#elif defined(win32) || defined(win64)
    comando = ((QString)"explorer " + posizione).toUtf8().constData();
#endif
    QFuture<void> future1 = QtConcurrent::run(&mostra, comando);
}


static void mostra(const char *comando){
    system(comando);
}
