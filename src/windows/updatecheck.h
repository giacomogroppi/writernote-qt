#pragma once

#define POSIZIONEPATHVERSIONE ":/image/src/windows/versione.xml"
#define SITOGIT "https://api.github.com/repos/giacomogroppi/writernote-qt/contents/src/windows/versione.xml"

#define NOMECONTENT "\"content\""
#define NOMEFINE "encoding"

#ifndef VERSION_SNAPCRAFT
#define NO_VER_DEF "[no version available]"
#endif

#include <QAction>
#include <QNetworkReply>
#include "Scheduler/WObject.h"

class updatecheck: public WObject
{
    Q_OBJECT
public:
    updatecheck(WObject *parent, std::function<void(const QString &message, const QString &version)> showDialog,
                std::function<void(bool)> setVisibleUpdateButton);
    ~updatecheck();

    void checkupdate();

    void sslErrors(QNetworkReply *, const WListFast<QSslError> &errors);
    QNetworkAccessManager *manager;
    QNetworkRequest request;


    QNetworkReply *reply;

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
    std::function<void(const QString &message, const QString &version)> _showDialog;
    std::function<void(bool)> _setVisibleUpdateButton;
private slots:
    void managerFinished();

private:

    void start();

signals:
    /** result == true there are update */
    void result(bool result);
};

