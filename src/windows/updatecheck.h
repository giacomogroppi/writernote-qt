#pragma once

#define POSIZIONEPATHVERSIONE ":/image/src/windows/versione.xml"
#define SITOGIT "https://api.github.com/repos/giacomogroppi/writernote-qt/contents/src/windows/versione.xml"

#define NOMECONTENT "\"content\""
#define NOMEFINE "encoding"

#ifndef VERSION_SNAPCRAFT
#define NO_VER_DEF "[no version available]"
#endif

#include "Scheduler/WObject.h"
#include "core/WString.h"

class updatecheck: public WObject
{
public:
    updatecheck(WObject *parent, std::function<void(const WString &message, const WString &version)> showDialog,
                std::function<void(bool)> setVisibleUpdateButton);
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
    std::function<void(const WString &message, const WString &version)> _showDialog;
    std::function<void(bool)> _setVisibleUpdateButton;

#if defined(USE_QT)
    void sslErrors(QNetworkReply *, const WListFast<QSslError> &errors);
    QNetworkAccessManager *manager;
    QNetworkRequest request;


    QNetworkReply *reply;

private slots:
    void managerFinished();
signals:
    /** result == true there are update */
    void result(bool result);

#endif // USE_QT

private:

    void start();
};

