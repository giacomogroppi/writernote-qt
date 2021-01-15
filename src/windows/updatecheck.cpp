#if defined(WIN32) || defined(WIN64) || defined(TESTING)
#include "updatecheck.h"

#include "stdlib.h"
#include "string.h"
#include "stdio.h"

//#include <cpr/cpr.h>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QNetworkAccessManager>

#include "mostra_finestra_i.h"
#include <QFile>
#include <QTextStream>

#include <QMessageBox>
#include "../areyousure.h"

#include <QJsonArray>
#include <QJsonDocument>

#include "../dialog_critic.h"

//using Base64  = macaron::Base64;

void updatecheck::start(){
    QFile file(POSIZIONEPATHVERSIONE);

    if(!file.open(QIODevice::ReadOnly)) {
        this->currentversione = -1;
        return;
    }

    bool ok = false;
    QTextStream in(&file);

    while(!in.atEnd()) {
        this->currentversione = in.readLine().toUInt(&ok);
    }


    file.close();

    if(this->currentversione == -1)
        return;

    manager = new QNetworkAccessManager();
    request.setUrl(QUrl(SITOGIT));

    reply = manager->get(request);

    QObject::connect(reply, &QNetworkReply::finished, this, &updatecheck::managerFinished);
}

updatecheck::updatecheck()
{
    this->start();
}


static QString decode_frombase64(QString stringa){
    QByteArray b(stringa.toUtf8().constData());

    return b.fromBase64(b);
}


void updatecheck::managerFinished(){
    if(reply->error()){
        dialog_critic("We had a problem with internet connection");
        delete manager;
        return;
    }

    QString testo = reply->readAll();

    QJsonDocument doc = QJsonDocument::fromJson(testo.toUtf8());

    bool check;

    testo = decode_frombase64(doc["content"].toString());

    testo = testo.mid(0, testo.length()-1);

    int version = testo.toInt(&check);

    if(!check){
        return dialog_critic("We had a problem while reading the current version");
    }

    if(version < this->currentversione){
        mostra = false;
        return this->mostrafinestra();
    }

    if(mostra){
        messaggio_utente("There is no update available");
    }
}


void updatecheck::mostrafinestra(){
    if(areyousure(nullptr, "Update Writernote", "Do you want to update writernote?")){
        mostra_finestra_i("https://github.com/giacomogroppi/writernote-qt/releases");
    }
}

void updatecheck::restart(){
    this->start();
    this->mostra = true;
}

#endif //win32
