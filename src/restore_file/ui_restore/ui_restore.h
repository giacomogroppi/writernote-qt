#ifndef UI_RESTORE_H
#define UI_RESTORE_H

#include <QDialog>

namespace Ui {
class ui_restore;
}

class ui_restore : public QDialog
{
    Q_OBJECT

public:
    explicit ui_restore(QWidget *parent = nullptr);
    ~ui_restore();

private:
    Ui::ui_restore *ui;
};

#endif // UI_RESTORE_H
