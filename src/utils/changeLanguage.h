#pragma once


#include <QCoreApplication>

class WString;

class language_manager{
public:
    static WString loadLastLanguage();
    static void saveCurrentLanguage(WString &language);

    static void setLanguage(QCoreApplication *app);
};

