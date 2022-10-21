// (c) 2017 Ekkehard Gentz (ekke) @ekkescorner
// my blog about Qt for mobile: http://j.mp/qt-x
// see also /COPYRIGHT and /LICENSE

#pragma once

//#include <QtAndroid>

#include <QJniObject>
#include <QtCore/private/qandroidextras_p.h>

#include "shareutils.h"

class AndroidShareUtils : public PlatformShareUtils, public QAndroidActivityResultReceiver
{
public:
    AndroidShareUtils(QObject* parent = nullptr);
    bool checkMimeTypeView(const QString &mimeType) override;
    bool checkMimeTypeEdit(const QString &mimeType) override;
    void share(const QString &text, const QUrl &url) override;
    void sendFile(const QString &filePath, const QString &title, const QString &mimeType, const int &requestId, const bool &altImpl) override;
    void viewFile(const QString &filePath, const QString &title, const QString &mimeType, const int &requestId, const bool &altImpl) override;
    void editFile(const QString &filePath, const QString &title, const QString &mimeType, const int &requestId, const bool &altImpl) override;

    void handleActivityResult(int receiverRequestCode, int resultCode, const QJniObject &data) override;
    void onActivityResult(int requestCode, int resultCode);

    void checkPendingIntents(const QString workingDirPath) override;

    static AndroidShareUtils* getInstance();

public slots:
    void setFileUrlReceived(const QString &url);
    void setFileReceivedAndSaved(const QString &url);
    bool checkFileExits(const QString &url);

private:
    bool mIsEditMode;
    qint64 mLastModified;
    QString mCurrentFilePath;

    static AndroidShareUtils* mInstance;

    void processActivityResult(int requestCode, int resultCode);

};

