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

};

#endif // UI_SCROLL_H
