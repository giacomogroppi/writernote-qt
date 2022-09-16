#pragma once


#include <QCoreApplication>

class QString;

class language_manager{
public:
    static QString loadLastLanguage();
    static void saveCurrentLanguage(QString &language);

    static void setLanguage(QCoreApplication *app);
};

