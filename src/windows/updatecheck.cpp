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
#include "utils/areyousure/areyousure.h"
#include <QJsonArray>
#include <QJsonDocument>
#include "utils/dialog_critic/dialog_critic.h"
#include "mainwindow.h"
#include "showmessageupdate.h"
#include "utils/platform.h"

#define POSNAME "name"

static updatecheck::n_priority priority(QJsonDocument &doc, QString &update, const char *c_ver)
{
    const auto critical = QChar::fromLatin1(updatecheck::high);
    if(update.indexOf(critical) != -1)
        return updatecheck::critical;

    int i, len;

    auto temp_2 = doc.toJson();

    bool find = false;

    for(i=0, len = temp_2.length(); i<len and doc[i][POSNAME].toString() != c_ver; i++){
        if(doc[i][POSNAME].toString().indexOf(critical) != -1)
            return updatecheck::critical;

        if(doc[i][POSNAME].toString().indexOf(critical) != -1)
            find = true;

    }

    if(find)
        return updatecheck::high;

    return updatecheck::low;

}

void updatecheck::start()
{
    manager = new QNetworkAccessManager();
    request.setUrl(QUrl("https://api.github.com/repos/giacomogroppi/writernote-qt/tags"));
    reply = manager->get(request);

    QObject::connect(reply, &QNetworkReply::finished, this, &updatecheck::managerFinished);
}

updatecheck::updatecheck(QAction *a):
    QObject(NULL)
{
    action = a;

    this->start();
}

updatecheck::~updatecheck()
{
    if(manager){
        delete manager;
        manager = NULL;
    }
}

void updatecheck::managerFinished()
{
    QString __mess;
    QString testo;
    QJsonDocument doc;

#ifndef VERSION_SNAPCRAFT
    if(mostra)
        dialog_critic("This version of writernote was not compiled following the \ninstructions for setting the version, \nso I can't tell if there are any updates");
    return;
#endif

    if(reply->error()){
        if(mostra)
#if defined(unix)
            dialog_critic("We had a problem with internet connection " + reply->errorString());
#elif defined(WIN32) || defined(WIN64) || defined(MACOS)
            dialog_critic("We had a problem with internet connection " + reply->errorString() + "\nReinstall the app, or download the ssl libraries.");
#endif
        delete manager;
        manager = NULL;
        return;
    }

    testo = reply->readAll();

    doc = QJsonDocument::fromJson(testo.toUtf8());

    testo = doc[0][POSNAME].toString();

    if(VERSION_STRING != testo and testo.toUpper() != "TESTING"
            and QString(VERSION_STRING).toUpper() != "TESTING"){
        auto res = priority(doc, testo, VERSION_STRING);

        if(res == n_priority::critical){
            __mess = "There is a very important update to do";
        }else if(res == n_priority::high){
            __mess = "There is an importat update to do";
        }else{
            __mess = "There is an update to do";
        }

        mostra = false;
        if(action)
            action->setVisible(true);

        ShowMessageUpdate show(nullptr, __mess, testo);

        show.exec();
        return;
    }

    if(mostra){
        user_message("There is no update available");
    }
}

void updatecheck::restart()
{
    this->start();
    this->mostra = true;
}
