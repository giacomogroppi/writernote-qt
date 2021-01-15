#ifndef UPDATECHECK_H
#define UPDATECHECK_H

#define POSIZIONEPATHVERSIONE ":/image/src/windows/versione.xml"
#define SITOGIT "https://api.github.com/repos/giacomogroppi/writernote-qt/contents/src/windows/versione.xml"

#define NOMECONTENT "\"content\""
#define NOMEFINE "encoding"

#include <QNetworkReply>
#include <QObject>

class updatecheck: public QObject
{
    Q_OBJECT
public:
    updatecheck();
    int currentversione;

    void checkupdate();
    void mostrafinestra();

    void sslErrors(QNetworkReply *, const QList<QSslError> &errors);
    QNetworkAccessManager *manager;
    QNetworkRequest request;

    QNetworkReply *reply;

    bool ok = false;

    ~updatecheck(){ };


private slots:
    void managerFinished();

};

#endif // UPDATECHECK_H
