#ifndef UI_OPTION_COPYBOOK_H
#define UI_OPTION_COPYBOOK_H

#include <QDialog>

namespace Ui {
class ui_option_copybook;
}

class ui_option_copybook : public QDialog
{
    Q_OBJECT

public:
    explicit ui_option_copybook(QWidget *parent = nullptr);
    ~ui_option_copybook();

private:
    Ui::ui_option_copybook *ui;
};

#endif // UI_OPTION_COPYBOOK_H
