#ifndef COLOR_PIXMAP_H
#define COLOR_PIXMAP_H

#include <QWidget>
#include <QPixmap>
#include <QPen>

#include "style_struct.h"

class color_pixmap : public QWidget
{
    Q_OBJECT
public:
    explicit color_pixmap(QWidget *parent = nullptr);

    void set(style_struct_S *);



private:
    style_struct_S *style;

    QPixmap pixmap;
    QColor colore = Qt::white;

    QPen penna;

signals:


protected:
    void paintEvent(QPaintEvent *event) override;
};

#endif // COLOR_PIXMAP_H
