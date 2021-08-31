#ifndef BUTTON_SIZE_H
#define BUTTON_SIZE_H

#include <QDialog>

namespace Ui {
class button_size;
}

class button_size : public QDialog
{
    Q_OBJECT

public:
    explicit button_size(QWidget *parent = nullptr, int size = 20);
    ~button_size();

private:
    Ui::button_size *ui;
signals:
    void update_size(int val);
private slots:
    void on_pushButton_clicked();
    void on_horizontalSlider_valueChanged(int value);
};

#endif // BUTTON_SIZE_H
