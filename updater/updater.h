#ifndef UPDATER_H
#define UPDATER_H

#include <QMainWindow>
#include <QString>

#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QList>
#include <QSslError>
#include <QProcess>

QT_BEGIN_NAMESPACE
namespace Ui { class updater; }
QT_END_NAMESPACE

class updater : public QMainWindow
{
    Q_OBJECT

public:
    updater(QWidget *parent = nullptr);
    ~updater();

    bool downloadFile(const QString &url, const QString &dest);
    bool extractFile(const QString &path, const QString &dest);

    bool exe(const QList<QString> &argv,
            const int time);

    bool createDirectory(const QString &path);
    bool removeDirectory(const QString &path);
    bool cleanDirectory(const QString &path);

    bool moveWithA(const QString &from, const QString to);
    bool removeFile(const QString &path);

private slots:
    void downloadUpdate();
private:

    void sslErrors(QNetworkReply *, const QList<QSslError> &errors);
    QNetworkAccessManager *manager;
    QNetworkRequest request;


    QNetworkReply *reply;

    Ui::updater *ui;
};
#endif // UPDATER_H
