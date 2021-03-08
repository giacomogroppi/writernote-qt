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

        b = new currenttitle_class;

        this->copy(b, parent->m_currenttitle);
    }

    ~spacchettamento();

    void esecuzione();

private:
    MainWindow *m_parent;

    void copy(currenttitle_class *dest, currenttitle_class *src);

    /*
     * in case the user quit the window, we need to reset to the currenttitle we receve
     * we work only on b [NOT THE CURRENT TITLE PASS]
    */
    currenttitle_class *b;

signals:
    void progress(int);
    void finished();

public slots:
    void exit();

};

#endif // SPACCHETTAMENTO_H
