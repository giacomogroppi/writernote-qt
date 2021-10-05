#include "mostra_explorer.h"

#include "string.h"
#include <QFuture>
#include <QtConcurrent/QtConcurrent>

#include "../../windows/mostra_finestra_i.h"
#include "../dialog_critic/dialog_critic.h"
#include "../slash/slash.h"
#include <QDesktopServices>

#if defined(WIN32) || defined(WIN64) || defined(__OS2__) || defined(SNAP)
static int mostra(const char *comando);
#endif // WIN

#if defined(unix) && !defined(MACOS)
#define APPLICATION_NAME (QString)"xdg-open "
#elif defined(WIN32) || defined(WIN64) || defined(__OS2__)
#define APPLICATION_NAME (QString)"explorer.exe "
static void replace(char *data);
#elif defined(MACOS)
#define APPLICATION_NAME (QString)"open "
#elif defined(MACOS)
#define APPLICATION_NAME (QString)"open "
#endif

static QString remove_file(const QString &path);

void mostra_explorer(const QString &posizione)
{
#if defined(WIN32) || defined(WIN64) || defined(__OS2__)
    const char *comando;
    comando = (APPLICATION_NAME + posizione).toUtf8().constData();
    if(mostra(comando)){
        dialog_critic("We had a problem opening " + APPLICATION_NAME);
    }

#elif (defined(unix) || defined(MACOS)) && !defined(SNAP)
    QString tmp;
    tmp = remove_file(posizione);
    mostra_finestra_i(tmp);
#elif defined(SNAP)
    const QString path = remove_file(posizione);
    const QString tmp = QString("%1 %2").arg(APPLICATION_NAME).arg(path);
    mostra(tmp.toUtf8().constData());
#endif //SNAP
}

#if defined(WIN32) || defined(WIN64) || defined(__OS2__) || defined(SNAP)
static int mostra(const char *comando){
    return system(comando);
}
#endif

static QString remove_file(const QString &path){
    const char slash = slash::__slash();
    const int indexLast = path.lastIndexOf(slash);

    return path.mid(0, indexLast);

}
