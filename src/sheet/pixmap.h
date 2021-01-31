#ifndef PIXMAP_H
#define PIXMAP_H

#include <QPixmap>
#include <QWidget>

#include "style_struct.h"

#include <QColor>

class pixmap: public QWidget{
    Q_OBJECT

public:
    explicit pixmap(QWidget *parent = nullptr);

    QPixmap pixmap_;


    style_struct_S *style;

    QColor color;

    void set(int *);

protected:
    void paintEvent(QPaintEvent *event) override;
};

#endif // PIXMAP_H
