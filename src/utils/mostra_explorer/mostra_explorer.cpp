#include "mostra_explorer.h"

#include <QString>
#include <QFuture>
#include <QtConcurrent/QtConcurrent>

static void mostra(const char *comando);

#ifdef unix
#define APPLICATION_NAME "nautilus "
#elif defined(win32) || defined(win64)
#define APPLICATION_NAME "explorer "
#endif

void mostra_explorer(const char *posizione)
{
    const char *comando;

    comando = ((QString)APPLICATION_NAME + posizione).toUtf8().constData();

    QFuture<void> future1 = QtConcurrent::run(&mostra, comando);
}


static void mostra(const char *comando){
    system(comando);
}
