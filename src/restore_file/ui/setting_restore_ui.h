#ifndef SETTING_RESTORE_UI_H
#define SETTING_RESTORE_UI_H

#include <QDialog>

namespace Ui {
class setting_restore_ui;
}

class setting_restore_ui : public QDialog
{
    Q_OBJECT

public:
    explicit setting_restore_ui(QWidget *parent = nullptr);
    ~setting_restore_ui();

private:
    Ui::setting_restore_ui *ui;
};

#endif // SETTING_RESTORE_UI_H
