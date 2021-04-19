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

private:
    Ui::ui_scroll *ui;
};

#endif // UI_SCROLL_H
