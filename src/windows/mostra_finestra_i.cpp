#include "mostra_finestra_i.h"

#ifdef USE_QT

#include <QDesktopServices>
#include <QUrl>

void mostra_finestra_i (const WString &website)
{
    QUrl url = QUrl(QString(website.toUtf8().constData()), QUrl::TolerantMode);
    QDesktopServices::openUrl(url);
}

#endif
