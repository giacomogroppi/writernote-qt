#ifndef UI_SCROLL_H
#define UI_SCROLL_H

#include <QDialog>

namespace Ui {
class ui_scroll;
}

class ui_scroll : public QDialog
{
    Q_OBJECT

public:
    explicit ui_scroll(QWidget *parent = nullptr);
    ~ui_scroll();

    void reset(uint enable, int speed);

private:
    Ui::ui_scroll *ui;

signals:

    void updateData(uchar enable, int speed);

private slots:
    void on_pushButton_cancel_clicked();
    void on_pushButton_ok_clicked();
    void on_spinBox_valueChanged(int arg1);
    void on_checkBox_stateChanged(int arg1);
};

#endif // UI_SCROLL_H
