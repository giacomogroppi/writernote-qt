#ifndef DIALOG_SHEET_H
#define DIALOG_SHEET_H

#include <QDialog>

#include <QCloseEvent>
#include <QGraphicsScene>

#include <QListWidgetItem>

#include "style_struct.h"

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

    void on_listWidget_itemClicked(QListWidgetItem *item);

    void on_listWidget_itemDoubleClicked(QListWidgetItem *item);

    void on_pushButton_clicked();

    void on_pushButton_3_clicked();

    void on_pushButton_2_clicked();

private:

    void hide(bool);

    Ui::dialog_sheet *ui;


    QPixmap pixmapColor, pixmapDraw;

    style_struct style_element;
    int current = 0;

    QPalette pal;
    QPen penna;

    void draw();

    QGraphicsScene *m_graphicsScene;

    void updateList();
    void setValue();
protected:
    void resizeEvent(QResizeEvent *event) override;

};

#endif // DIALOG_SHEET_H
