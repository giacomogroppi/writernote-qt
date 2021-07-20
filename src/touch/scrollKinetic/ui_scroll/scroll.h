#ifndef SCROLL_H
#define SCROLL_H

#include <QDialog>

namespace Ui {
class scroll;
}

class scroll : public QDialog
{
    Q_OBJECT

public:
    explicit scroll(QWidget *parent = nullptr);
    ~scroll();

    void reset(uint enable, int speed);

private:
    Ui::scroll *ui;

signals:

    void updateData(uchar enable, int speed);

private slots:
    void on_pushButton_cancel_clicked();
    void on_pushButton_ok_clicked();
    void on_spinBox_valueChanged(int arg1);
    void on_checkBox_stateChanged(int arg1);
};

#endif // SCROLL_H
