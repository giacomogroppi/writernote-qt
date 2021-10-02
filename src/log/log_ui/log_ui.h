#ifndef LOG_UI_H
#define LOG_UI_H

#include <QWidget>
#include <QCloseEvent>
#include <QString>
#include <QDialog>

#define NAME_LOG_EXT log_write


namespace Ui {
class log_ui;
}

class log_ui : public QDialog
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

    void write(const QString &message,
               enum type_write type);

private:
    static void addTime(QString &message);
    Ui::log_ui *ui;

    enum permi: int {
        error, /* error */
        disable, /* log disable */
        enable /* ok */
    };

    QString getNameLog();

    permi m_permi = permi::enable;

    QString pos_log = "";

    void saveData();
    int loadData();

    void updateAll();

    /* this function manage all error */
    void updateError();

protected:
    void closeEvent(QCloseEvent *) override;

private slots:
    void on_button_close_clicked();
    void on_button_enable_log_clicked();
    void on_Button_open_log_clicked();
    void on_button_change_position_clicked();
};

extern log_ui *NAME_LOG_EXT;
#define LOG(message, mode) NAME_LOG_EXT->write(message, mode)

#endif // LOG_UI_H
