#include "mostra_explorer.h"

#include "string.h"
#include <QFuture>
#include <QtConcurrent/QtConcurrent>

#include "../../windows/mostra_finestra_i.h"
#include "../dialog_critic/dialog_critic.h"
#include "../slash/slash.h"
#include <QDesktopServices>

#if defined(WIN32) || defined(WIN64) || defined(__OS2__)
static int mostra(const char *comando);
#endif // WIN

#if defined(unix) && !defined(MACOS)
#define APPLICATION_NAME (QString)"nautilus "
#elif defined(WIN32) || defined(WIN64) || defined(__OS2__)
#define APPLICATION_NAME (QString)"explorer.exe "
static void replace(char *data);
#elif defined(MACOS)
#define APPLICATION_NAME (QString)"open "
#elif defined(MACOS)
#define APPLICATION_NAME (QString)"open "
#endif

static QString remove_file(const QString &path);

void mostra_explorer(QString posizione)
{
    /*
     * in windows we don't need to run it in an other thread
    */
#if defined(WIN32) || defined(WIN64) || defined(__OS2__)
    const char *comando;
    comando = (APPLICATION_NAME + posizione).toUtf8().constData();
    if(mostra(comando)){
        dialog_critic("We had a problem opening " + APPLICATION_NAME);
    }

#elif unix || MACOS
    posizione = remove_file(posizione.toUtf8().constData());
    mostra_finestra_i(posizione);
    //QFuture<void> future1 = QtConcurrent::run(&mostra, comando);
#endif

}

#if defined(WIN32) || defined(WIN64) || defined(__OS2__)
static int mostra(const char *comando){
    return system(comando);
}
#endif

static QString remove_file(const QString &path){
    const char slash = slash::__slash();
    const int indexLast = path.lastIndexOf(slash);

    return path.mid(0, indexLast);

}
