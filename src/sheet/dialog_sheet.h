#ifndef DIALOG_SHEET_H
#define DIALOG_SHEET_H

#include <QDialog>

#include "pixmap.h"
#include "color_pixmap.h"

#include <QCloseEvent>

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


    void closeEvent (QCloseEvent *event);

private:
    Ui::dialog_sheet *ui;

    pixmap *pixmapd;

    color_pixmap *pixmaps;

    style_struct style;
    int current = 0;
};

#endif // DIALOG_SHEET_H
