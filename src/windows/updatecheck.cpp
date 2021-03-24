#include "updatecheck.h"

#include "stdlib.h"
#include "string.h"
#include "stdio.h"

#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QNetworkAccessManager>

#include "mostra_finestra_i.h"
#include <QFile>
#include <QTextStream>

#include <QMessageBox>
#include "../utils/areyousure/areyousure.h"

#include <QJsonArray>
#include <QJsonDocument>

#include "../utils/dialog_critic/dialog_critic.h"
#include "../mainwindow.h"

static updatecheck::n_priority priority(QJsonDocument &doc, QString &update, const char *c_ver);

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
    //request.setUrl(QUrl(SITOGIT));
    request.setUrl(QUrl("https://api.github.com/repos/giacomogroppi/writernote-qt/tags"));
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

/* //last update check
void updatecheck::managerFinished(){
    if(reply->error()){
        if(mostra)
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

    if(!check && mostra){
        return dialog_critic("We had a problem while reading the current version");
    }

    if(version < this->currentversione){
        mostra = false;
        return this->mostrafinestra();
    }

    if(mostra){
        messaggio_utente("There is no update available");
    }
}*/

#define POSNAME "name"

void updatecheck::managerFinished(){
#ifndef VERSION_SNAPCRAFT
    if(mostra)
        dialog_critic("This version of writernote was not compiled following the \ninstructions for setting the version, \nso I can't tell if there are any updates");
    return;
#endif

    if(reply->error()){
        if(mostra)
            dialog_critic("We had a problem with internet connection " + reply->errorString());
        delete manager;
        return;
    }

    QString testo = reply->readAll();

    QJsonDocument doc = QJsonDocument::fromJson(testo.toUtf8());

    testo = doc[0][POSNAME].toString();

    if(VERSION_STRING != testo){
        auto res = priority(doc, testo, VERSION_STRING);

        if(res == n_priority::critical){
            messaggio_utente("There is a very important update to do");
        }else if(res == n_priority::high){
            messaggio_utente("There is an importat update to do");
        }

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

/*
 * the function find in doc if there is a tag with critical or hight update
*/
static updatecheck::n_priority priority(QJsonDocument &doc, QString &update, const char *c_ver){
    QString temp;
    if(update.indexOf(updatecheck::critical) != -1)
        return updatecheck::critical;


    int i, len;

    auto temp_2 = doc.toJson();

    bool find = false;

    for(i=0, len = temp_2.length(); i<len && doc[i][POSNAME].toString() != c_ver; i++){
        if(doc[i][POSNAME].toString().indexOf(updatecheck::critical) != -1)
            return updatecheck::critical;

        if(doc[i][POSNAME].toString().indexOf(updatecheck::high) != -1)
            find = true;

    }

    if(find)
        return updatecheck::high;

    return updatecheck::low;

}
