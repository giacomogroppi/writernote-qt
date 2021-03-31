#include "mostra_explorer.h"

#include "string.h"
#include <QFuture>
#include <QtConcurrent/QtConcurrent>

#include "../../windows/mostra_finestra_i.h"

static void mostra(const char *comando);

#if defined(unix)
#define APPLICATION_NAME "nautilus "
#elif defined(WIN32) || defined(WIN64)
#define APPLICATION_NAME "explorer "
static void replace(char *data);
#endif

static QString remove_s(const char *);

void mostra_explorer(QString posizione)
{
    const char *comando;

    posizione = remove_s(posizione.toUtf8().constData());

    comando = ((QString)APPLICATION_NAME + posizione).toUtf8().constData();

    /*
     * in windows we don't need to run it in an other thread
    */
#if defined(WIN32) || defined(WIN64)
    mostra(comando);
#elif unix
    mostra_finestra_i(posizione.toUtf8().constData());
    //QFuture<void> future1 = QtConcurrent::run(&mostra, comando);
#endif

}


static void mostra(const char *comando){
    system(comando);
}

#define S '/'


static QString remove_s(const char *stringa){
    int i, len;

    len = strlen(stringa);

    for(i=len; i>0; i--)
        if(stringa[i] == S)
            break;

#if defined(WIN32) || defined(WIN64)
    /*
     * we can't modify the data return from .toutf8.constdata() [QString]
    */
    QString string_r;
    char * temp = new char [len];

    memcpy(temp, stringa, sizeof(char)*len);

    replace(temp);
    string_r = temp;

    delete [] temp;

    return string_r.mid(0, i);

#elif unix
    return QString::fromUtf8(stringa, i);
#endif

}

#if defined(WIN64) || defined(WIN32)
#define C '\\'

static void replace(char *data){
    for(int i=0; data[i] != '\0'; i++)
        if(data[i] == S)
            data[i] = C;

}
#endif
