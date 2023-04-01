#include "mostra_explorer.h"
#include "string.h"
#include <QFuture>
#include <QtConcurrent/QtConcurrent>
#include "windows/mostra_finestra_i.h"
#include "utils/slash/slash.h"
#include <QDesktopServices>
#include "../path/pathfile.h"
#include "utils/WCommonScript.h"
#include "utils/platform.h"

#if defined(WIN32) || defined(WIN64) || defined(__OS2__) || defined(SNAP)
static int mostra(const QString &comando);
#endif // WIN

#if defined(unix) and !defined(MACOS)
#define APPLICATION_NAME (QString)"xdg-open "
#elif defined(WIN32) || defined(WIN64) || defined(__OS2__)
#define APPLICATION_NAME (QString)"explorer.exe "
#elif defined(MACOS)
#define APPLICATION_NAME (QString)"open "
#elif defined(MACOS)
#define APPLICATION_NAME (QString)"open "
#endif

void mostra_explorer(const QString &posizione)
{
#if defined(WIN32) || defined(WIN64)
    QString tmp = pathFile::remove_file(posizione);
    tmp = APPLICATION_NAME + tmp;
    if(mostra(tmp)){
        dialog_critic("We had a problem opening " + APPLICATION_NAME);
    }

#elif (defined(unix) || defined(MACOS)) && !defined(SNAP)
    QString tmp = pathFile::remove_file(posizione);
    tmp = pathFile::remove_file(posizione);
    mostra_finestra_i(tmp);
#elif defined(SNAP)
    const QString path = pathFile::remove_file(posizione);
    const QString tmp = QString("%1 %2").arg(APPLICATION_NAME).arg(path);
    mostra(tmp);
#endif //SNAP
}

#if defined(WIN32) || defined(WIN64) || defined(__OS2__) || defined(SNAP)
static int mostra(const QString &comando){
    const auto res = system(comando.toUtf8().constData());

    if(is_windows){
        return res != 1;
    }

    return res;
}
#endif


