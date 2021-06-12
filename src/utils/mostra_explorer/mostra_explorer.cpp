#include "mostra_explorer.h"

#include "string.h"
#include <QFuture>
#include <QtConcurrent/QtConcurrent>

#include "../../windows/mostra_finestra_i.h"
#include "../dialog_critic/dialog_critic.h"
#include "../slash/slash.h"

static int mostra(const char *comando);

#if defined(unix)
#define APPLICATION_NAME "nautilus "
#elif defined(WIN32) || defined(WIN64) || defined(__OS2__)
#define APPLICATION_NAME (QString)"explorer.exe "
static void replace(char *data);
#endif

static QString remove_s(const char *);

void mostra_explorer(QString posizione)
{
    const char *comando;

    posizione = remove_s(posizione.toUtf8().constData());

    comando = (APPLICATION_NAME + posizione).toUtf8().constData();

    /*
     * in windows we don't need to run it in an other thread
    */
#if defined(WIN32) || defined(WIN64) || defined(__OS2__)
    if(mostra(comando)){
        dialog_critic("We had a problem opening " + APPLICATION_NAME);
    }

#elif unix
    mostra_finestra_i(posizione.toUtf8().constData());
    //QFuture<void> future1 = QtConcurrent::run(&mostra, comando);
#endif

}


static int mostra(const char *comando){
    return system(comando);
}

static QString remove_s(const char *stringa){
    int i, len;
    char *tmp;
    QString string_r;

    len = strlen(stringa);

    for(i=len; i>0; i--)
        if(stringa[i] == slash::__slash())
            break;

#if defined(WIN32) || defined(WIN64) || defined(__OS2__)
    /*
     * we can't modify the data return from .toutf8.constdata() [QString]
    */
    tmp = new char [len];

    memcpy(tmp, stringa, sizeof(char)*len);

    string_r = tmp;

    delete [] tmp;

    return string_r.mid(0, i);

#elif unix
    return QString::fromUtf8(stringa, i);
#endif

}
