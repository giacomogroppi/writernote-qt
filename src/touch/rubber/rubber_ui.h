#ifndef RUBBER_UI_H
#define RUBBER_UI_H

#include <QDialog>

namespace Ui {
class rubber_ui;
}

class rubber_ui : public QDialog
{
    Q_OBJECT

public:
    explicit rubber_ui(QWidget *parent = nullptr);
    ~rubber_ui();

private:
    Ui::rubber_ui *ui;

protected:
    bool event(QEvent *) override;
};

#endif // RUBBER_UI_H
