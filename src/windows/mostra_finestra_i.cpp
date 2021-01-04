#include "mostra_finestra_i.h"

#include <QDesktopServices>
#include <QUrl>

void mostra_finestra_i(const char *sito){
    QDesktopServices::openUrl(QUrl(sito, QUrl::TolerantMode));
}
