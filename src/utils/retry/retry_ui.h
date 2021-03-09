#ifndef RETRY_UI_H
#define RETRY_UI_H

#include <QDialog>

namespace Ui {
class retry_ui;
}

class retry_ui : public QDialog
{
    Q_OBJECT

public:
    explicit retry_ui(QWidget *parent = nullptr);
    ~retry_ui();

private:
    Ui::retry_ui *ui;
};

#endif // RETRY_UI_H
