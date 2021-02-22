#ifndef SPACCHETTAMENTO_H
#define SPACCHETTAMENTO_H

#include "../mainwindow.h"

//void spacchettamento(MainWindow *parent);
#include <QObject>

class spacchettamento: public QObject{
    Q_OBJECT

public:
    spacchettamento(MainWindow *parent){
        m_parent = parent;
    }

    void esecuzione();

private:
    MainWindow *m_parent;

signals:
    void progress(int);
    void finished();

};

#endif // SPACCHETTAMENTO_H
