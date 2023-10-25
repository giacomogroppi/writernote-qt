#include "updatecheck.h"
#include "stdlib.h"
#include "string.h"
#include "stdio.h"

#include "mostra_finestra_i.h"
#include "utils/areyousure/areyousure.h"
#include "utils/platform.h"
#define POSNAME "name"

#ifdef USE_QT
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QNetworkAccessManager>
#include <QJsonArray>
#include <QJsonDocument>
#include <QFile>
#include <QTextStream>

static updatecheck::n_priority priority(QJsonDocument &doc, WString &update, const char *c_ver)
{
    const auto critical = QChar::fromLatin1(updatecheck::high).toLatin1();
    if(update.indexOf(critical) != -1)
        return updatecheck::critical;

    int i, len;

    auto temp_2 = doc.toJson();

    bool find = false;

    for(i=0, len = temp_2.size(); i < len and doc[i][POSNAME].toString() != c_ver; i++){
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

    QObject::connect(reply, &QNetworkReply::finished, [this] () {
        this->managerFinished();
    });
    //QObject::connect(reply, &QNetworkReply::finished, this, &updatecheck::managerFinished);
}

updatecheck::updatecheck(WObject *parent,
                         Fn<void(const WString &message, const WString &version)> showDialog,
                         Fn<void(bool)> setVisibleUpdateButton)
    : WObject(parent)
    , _showDialog(showDialog)
    , _setVisibleUpdateButton(setVisibleUpdateButton)
{
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
    WString __mess;
    WString testo;
    QJsonDocument doc;

    if(reply->error()){
        if(mostra)
#if defined(unix)
            this->_showDialog(qstr("We had a problem with internet connection %1").arg(reply->errorString()), "");
#elif defined(WIN32) || defined(WIN64) || defined(MACOS)
            dialog_critic("We had a problem with internet connection " + reply->errorString() + "\nReinstall the app, or download the ssl libraries.");
#endif
        delete manager;
        manager = nullptr;
        return;
    }

    testo = reply->readAll().constData();

    doc = QJsonDocument::fromJson(testo.toUtf8().constData());

    testo = doc[0][POSNAME].toString();

#define VERSION_STRING "TESTING"
    if(testo != VERSION_STRING and testo.toUpper() != "TESTING"
            and WString(VERSION_STRING).toUpper() != "TESTING"){
        auto res = priority(doc, testo, VERSION_STRING);

        if(res == n_priority::critical){
            __mess = "There is a very important update to do";
        }else if(res == n_priority::high){
            __mess = "There is an importat update to do";
        }else{
            __mess = "There is an update to do";
        }

        mostra = false;
        this->_setVisibleUpdateButton(true);
        this->_showDialog(__mess, testo);
    } else {
        this->_showDialog("This is the latest release", testo);
    }
}

void updatecheck::restart()
{
    this->start();
    this->mostra = true;
}
#endif // USE_QT
