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
    explicit log_ui(QWidget * parent,
                    QString * pos_log);
    ~log_ui();

private:
    Ui::log_ui *ui;

    QString * pos_log;


protected:
    void closeEvent(QCloseEvent *) override;

signals:
    /* emit when the user want to change location of log */
    void changePosition(QString);

};

#endif // LOG_UI_H
