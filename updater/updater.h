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

    bool downloadFile(QString url, const QString dest);
    bool extractFile(const QString &path, const QString &dest);

    static bool removeDirectory(const QString &path);

    bool moveWithA(QString l, const QString to);
    bool removeFile(QString l);

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
