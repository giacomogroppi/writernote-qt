#ifndef MAKE_DEFAULT_UI_H
#define MAKE_DEFAULT_UI_H

#include <QDialog>

namespace Ui {
class make_default_ui;
}

class make_default_ui : public QDialog
{
    Q_OBJECT

public:
    explicit make_default_ui(QWidget *parent = nullptr,
                             QString text = "",
                             QString title = "");
    ~make_default_ui();

private slots:
    void on_button_no_clicked();

    void on_button_yes_clicked();

private:
    bool alreadyDecide = false;
    Ui::make_default_ui *ui;

signals:
    /* emit true if is check remember my choise */
    void no(bool);
    void yes(bool);
};

#endif // MAKE_DEFAULT_UI_H
