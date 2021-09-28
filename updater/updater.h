#ifndef UPDATER_H
#define UPDATER_H

#include <QMainWindow>
#include <QString>

#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QList>
#include <QSslError>

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

    static bool createDirectory(const QString &path);
    static bool removeDirectory(const QString &path);
    static bool cleanDirectory(const QString &path);

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
