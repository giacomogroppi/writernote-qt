#ifndef TABLETAPPLICATION_H
#define TABLETAPPLICATION_H

#include <QApplication>

#include "tabletcanvas.h"
#include <QEvent>

class TabletApplication : public QApplication
{
    Q_OBJECT

public:
    using QApplication::QApplication;

    bool event(QEvent *event) override;
    void setCanvas(TabletCanvas *canvas)
        { m_canvas = canvas; }

private:
    TabletCanvas *m_canvas = nullptr;
};

#endif
