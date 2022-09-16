#pragma once

#include <QDialog>

namespace Ui {
class retry_ui;
}

class retry_ui : public QDialog
{
    Q_OBJECT

public:
    explicit retry_ui(QWidget *parent = nullptr, QString title = QString(), QString message = QString(), QString error = QString());
    ~retry_ui();

private:
    QString title, message, messageError;
    Ui::retry_ui *ui;

public slots:
    void resultRetry_reciver(bool check);

signals:
    void ui_retry();
    void ui_close();

private slots:
    void on_button_retry_clicked();
    void on_button_close_clicked();

protected:
    void closeEvent(QCloseEvent *);

};
