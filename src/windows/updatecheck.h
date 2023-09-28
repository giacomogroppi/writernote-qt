#pragma once

#define POSIZIONEPATHVERSIONE ":/image/src/windows/versione.xml"
#define SITOGIT "https://api.github.com/repos/giacomogroppi/writernote-qt/contents/src/windows/versione.xml"

#define NOMECONTENT "\"content\""
#define NOMEFINE "encoding"

#ifndef VERSION_SNAPCRAFT
#define NO_VER_DEF "[no version available]"
#endif

#include "Scheduler/WObject.h"
#include "core/String/WString.h"

#ifdef USE_QT
# include <QtNetwork/QNetworkAccessManager>
#endif // USE_QT

class updatecheck: public WObject
{
public:
    updatecheck(WObject *parent, Fn<void(const WString &message, const WString &version)> showDialog,
                Fn<void(bool)> setVisibleUpdateButton);
    ~updatecheck();

    void checkupdate();

    void restart();

    bool ok = false;
    bool mostra = false;


    /* importance of the update */
    enum n_priority: char{
        high = 'h',
        low = 'l',
        critical = 'c'
    };

private:
    Fn<void(const WString &message, const WString &version)> _showDialog;
    Fn<void(bool)> _setVisibleUpdateButton;

    DEFINE_LISTENER_0(managerFinished);
    /** result == true there are update */
    W_EMITTABLE_1(result, bool, result);

#if defined(USE_QT)
    void sslErrors(QNetworkReply *, const WListFast<QSslError> &errors);
    QNetworkAccessManager *manager;
    QNetworkRequest request;


    QNetworkReply *reply;

#endif // USE_QT

private:

    void start();
};

