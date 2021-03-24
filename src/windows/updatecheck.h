#ifndef UPDATECHECK_H
#define UPDATECHECK_H

#define POSIZIONEPATHVERSIONE ":/image/src/windows/versione.xml"
#define SITOGIT "https://api.github.com/repos/giacomogroppi/writernote-qt/contents/src/windows/versione.xml"

#define NOMECONTENT "\"content\""
#define NOMEFINE "encoding"

#ifndef VERSION_SNAPCRAFT
#define NO_VER_DEF "[no version available]"
#endif

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

    void restart();

    bool ok = false;
    bool mostra = false;
    ~updatecheck(){ };

    /* importance of the update */
    enum n_priority: char{
        high = 'h',
        low = 'l',
        critical = 'c'
    };

private slots:
    void managerFinished();

private:
    void start();
};

#endif // UPDATECHECK_H
