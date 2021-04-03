#ifndef DEFAULT_OPTION_UI_H
#define DEFAULT_OPTION_UI_H

#include <QDialog>
#include <QString>
#include <QCloseEvent>

namespace Ui {
class default_option_ui;
}

class default_option_ui : public QDialog
{
    Q_OBJECT

public:
    enum e_result: int{
        close,
        first,
        second,
        error
    };

    explicit default_option_ui(QWidget *parent = nullptr,
                               QString first_button = QString(),
                               QString second_button = QString());
    ~default_option_ui();

private slots:
    void on_button_cancel_clicked();

    void on_button_ok_clicked();

    void on_second_text_clicked();

    void on_first_text_clicked();

private:
    int res = -1;
    void updateStatus();
    Ui::default_option_ui *ui;

signals:
    void result(enum e_result);

protected:
    void closeEvent(QCloseEvent *event) override;

};

#endif // DEFAULT_OPTION_UI_H
