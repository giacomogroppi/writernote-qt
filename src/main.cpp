#include "mainwindow.h"

#include <QApplication>
#include "touch/tabletapplication.h"
#include "touch/tabletcanvas.h"

int main(int argc, char *argv[])
{
    TabletApplication app(argc, argv);
    TabletCanvas *canvas = new TabletCanvas;
    app.setCanvas(canvas);

    //QApplication a(argc, argv);
    MainWindow w(nullptr, canvas);

    if(argc != 1){
        w.setFile(argc-1, argv);
    }

    w.show();
    return app.exec();
    //return a.exec();

}
