#ifndef LOG_UI_H
#define LOG_UI_H

#include <QWidget>
#include <QCloseEvent>
#include <QString>

namespace Ui {
class log_ui;
}

class log_ui : public QWidget
{
    Q_OBJECT

public:
    explicit log_ui(QWidget * parent);
    ~log_ui();

    void showAll();

    enum type_write: int{
        critic_error,
        info,
        error_internal
    };

    void write(QString,
               enum type_write);

private:
    Ui::log_ui *ui;

    enum permi: int {
        error, /* error */
        disable, /* log disable */
        enable /* ok */
    };

    permi m_permi;

    QString pos_log;

    void saveData();
    int loadData();

    void updateAll();

    /* this function manage all error */
    void updateError();

protected:
    void closeEvent(QCloseEvent *) override;

signals:
    /* emit when the user want to change location of log */
    void changePosition(QString);

private slots:
    void on_button_close_clicked();
    void on_button_enable_log_clicked();
    void on_Button_open_log_clicked();
};

#endif // LOG_UI_H
