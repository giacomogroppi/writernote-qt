#include "updater.h"
#include "ui_updater.h"
#include "../src/utils/dialog_critic/dialog_critic.h"
#include "../src/utils/path/get_path.h"
#include "../src/utils/areyousure/areyousure.h"

#include <QFile>
#include <QDir>
#include <QDebug>
#include <QJsonDocument>

#define POS (QString)"C:\\Program Files (x86)\\writernote\\"

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

bool updater::downloadFile(QString url, const QString dest)
{
    int res;
    res = system("powershell echo hello");

    if(res){
        dialog_critic("It seams that you don't have installed powershell");
        return false;
    }

    if(QFile::exists(dest)){
        if(!QFile::remove(dest)){
            messaggio_utente("Before update writernote remove this file " + dest);
            return false;
        }
    }

    url = "Invoke-WebRequest -uri " + url + " -Method \"GET\"  -Outfile " + dest;
    url = "powershell " + url;

    res = system(url.toUtf8().constData());

    return !res;

}

bool updater::extractFile(QString l, const QString &dest)
{
    int res;
    QDir __dir(dest);

    if(__dir.exists()){
        if(!__dir.removeRecursively()){
            messaggio_utente("I can't update writernote becouse I can't remove this folder: " + dest);
            return false;
        }
    }

    l = "Expand-Archive -Path " + l + " -destinationPath " + dest;
    l = "powershell " + l;

    res = system(l.toUtf8().constData());

    return !res;
}

bool updater::moveWithA(QString l, const QString to)
{
    int res;

    l = "powershell Start-Process powershell -Verb runAs Move-Item -Path " + l + " -Destination " + to;

    res = system(l.toUtf8().constData());

    return !res;
}

bool updater::removeFile(QString l)
{
    int res;

    l = "powershell " + l;

    res = system(l.toUtf8().constData());

    return !res;

}

#define pos_installation POS + "\\writernote.exe"
void updater::downloadUpdate()
{
    QDir __dir(POS);
    QString testo, dest, ver;
    QJsonDocument doc;

    if(reply->error()){
        dialog_critic("We had this problem " + reply->errorString());
        delete manager;
        return;
    }

    if(!QFile::exists(POS)){
        if(!__dir.exists()){
            dialog_critic("I cant find writernote in " + POS);
            goto close;
        }
    }

    testo = reply->readAll();

    doc = QJsonDocument::fromJson(testo.toUtf8());

    dest = "C:" + (QString)get_path(path::home);
    dest += "\\Downloads\\writernote_setup.zip";

    ver = doc[0]["name"].toString();
    testo = "https://github.com/giacomogroppi/writernote-qt/releases/download/" + ver + "/writernote_setup_" + ver + ".zip";

    if(!downloadFile(testo, dest)){
        dialog_critic("I had a problem downloading the file");
        goto close;
    }

    /* to run powershell as amministrator use:
        Start-Process powershell -Verb runAs

        we need this for move object into c:\programs
    */

    testo = dest.mid(0, dest.indexOf(".zip"));

    if(!extractFile(dest, testo)){
        dialog_critic("We had a problem extracting the zip");
        goto close;
    }

    testo += "\\*";

    if(!moveWithA(testo, POS)){
        dialog_critic("We had a problem move writernote.exe to " + POS);
        goto close;
    }

    if(!areyousure("Remove file", "Do you want to remove the temporary file?")){
        goto close;
    }

    removeFile(testo);
    removeFile(dest);

    messaggio_utente("Writernote was updated successfully to version " + ver);

    close:
    this->close();
}

void updater::sslErrors(QNetworkReply *, const QList<QSslError> &errors)
{
    Q_UNUSED(errors);
    messaggio_utente("I can't check the current version becouse I had a problem\n");
}



