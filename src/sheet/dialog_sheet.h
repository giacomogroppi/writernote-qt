#ifndef DIALOG_SHEET_H
#define DIALOG_SHEET_H

#include <QDialog>

#include "pixmap.h"
#include "color_pixmap.h"

#include <QCloseEvent>
#include <QGraphicsScene>

QColor settaggiocolore(int *);

namespace Ui {
class dialog_sheet;
}

class dialog_sheet : public QDialog
{
    Q_OBJECT

public:
    explicit dialog_sheet(QWidget *parent = nullptr);
    ~dialog_sheet();

    void value_change();

private slots:
    void on_t_valueChanged(int arg1);

    void on_x_valueChanged(int arg1);

    void on_y_valueChanged(int arg1);


    void closeEvent (QCloseEvent *event) override;

    void on_pushButton_color_clicked();

private:
    Ui::dialog_sheet *ui;

    pixmap *pixmapd;

    color_pixmap *pixmaps;

    QPixmap pixmapColor, pixmapDraw;

    style_struct style;
    int current = 0;

    QPalette pal;
    QPen penna;

    void initPixmap();

    void draw();

    QGraphicsScene *m_graphicsScene;

protected:
    void paintEvent(QPaintEvent *event) override;
};

#endif // DIALOG_SHEET_H
