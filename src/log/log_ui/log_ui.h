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
#ifdef DEBUGINFO
        ,
        caller
#endif
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

// debug info
#ifdef DEBUGINFO
#define DEBUG_INFO_CALLER_SINGLE(caller) const char *caller
#define DEBUG_INFO_CALLER_MULTIPLE(caller) ,DEBUG_INFO_CALLER_SINGLE(caller)

/* mode is "start" or "stop" */
#define DEBUG_INFO_CALL_CALLER_PRIVATE(caller, mode) LOG(QString(caller)+ " "+ mode, log_ui::type_write::caller)
#define DEBUG_INFO_CALL_CALLER(caller) DEBUG_INFO_CALL_CALLER_PRIVATE(caller, "start")
#define DEBUG_INFO_END_CALLER(caller) DEBUG_INFO_CALL_CALLER_PRIVATE(caller, "stop")
#else
#define DEBUG_INFO_CALLER_SINGLE(caller)
#define DEBUG_INFO_CALLER_MULTIPLE(caller)
#define DEBUG_INFO_CALL_CALLER(caller)
#endif

// example
/*void foo(int arg1, int arg2, char **argv DEBUG_INFO_CALLER(caller)){
    DEBUG_INFO_CALL_CALLER(caller);
    DEBUG_INFO_END_CALLER(caller);
}

void main(void){
    foo(arg1, arg2, (char **)argv, CALLER("main"));
}
*/
#endif // LOG_UI_H
