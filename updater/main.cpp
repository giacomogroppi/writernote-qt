#include "updater.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    updater w;

    w.downloadUpdate();

    return a.exec();
}
