#ifndef CHANGELANGUAGE_H
#define CHANGELANGUAGE_H

#include <QCoreApplication>

class QString;

class language_manager{
public:
    static QString loadLastLanguage();
    static void saveCurrentLanguage(QString &language);

    static void setLanguage(QCoreApplication *app);
};

#endif // CHANGELANGUAGE_H
