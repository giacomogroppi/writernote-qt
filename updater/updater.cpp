#include "updater.h"
#include "ui_updater.h"
#include "../src/utils/dialog_critic/dialog_critic.h"
#include "../src/utils/path/get_path.h"

#define OK_POWER_EXT 0
#define OK_POWER_DOWN 0

#include <QFile>
#include <QDir>
#include <QJsonDocument>

updater::updater(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::updater)
{
    ui->setupUi(this);

    manager = new QNetworkAccessManager();
    request.setUrl(QUrl("https://api.github.com/repos/giacomogroppi/writernote-qt/tags"));
    reply = manager->get(request);

    QObject::connect(reply, &QNetworkReply::finished, this, &updater::downloadUpdate);

}

updater::~updater()
{
    delete ui;
}

bool updater::downloadFile(QString url, QString dest)
{
    int res;
    res = system("powershell");

    if(!res){
        dialog_critic("It seams that you don't have installed powershell");
        return false;
    }

    url = "Invoke-WebRequest -uri " + url + " -Method \"GET\"  -Outfile " + dest;

    res = system(url.toUtf8().constData());

    if(res != OK_POWER_DOWN){
        return false;
    }
    return true;

}

bool updater::exstractFile(QString l, const QString &dest)
{
    int res;

    l = "Expand-Archive -Path " + l + " -destinationPath " + dest;

    res = system(l.toUtf8().constData());

    if(res != OK_POWER_EXT){
        return true;
    }
    return false;
}

#define POS (QString)"C:\\Program Files (x86)\\writernote\\"

void updater::downloadUpdate()
{
    QDir __dir(POS + "\\writernote.exe");
    QString url, testo, dest;
    QJsonDocument doc;

    if(reply->error()){
        dialog_critic("We had this problem " + reply->errorString());
        delete manager;
        return;
    }

    /*if(!QFile::exists(POS)){
        if(!__dir.exists()){
            return dialog_critic("I cant find writernote in " + POS);
        }
    }*/

    testo = reply->readAll();

    doc = QJsonDocument::fromJson(testo.toUtf8());

    dest = get_path(path::home);
    dest += "\\Download\\writernote_setup.zip";

    testo = doc[0]["name"].toString();
    testo = "https://github.com/giacomogroppi/writernote-qt/releases/download/" + testo + "/writernote_setup_" + testo + ".zip";

    if(!downloadFile(url, dest)){
        dialog_critic("I had a problem downloading the file");
    }


}

void updater::sslErrors(QNetworkReply *, const QList<QSslError> &errors)
{
    messaggio_utente("I can't check the current version becouse I had a problem\n");
}



