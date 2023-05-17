#include "mostra_finestra_i.h"

#ifdef USE_QT

#include <QDesktopServices>
#include <QUrl>

void mostra_finestra_i(const WString &sito){
    QDesktopServices::openUrl(QUrl(sito, QUrl::TolerantMode));
}

#endif
