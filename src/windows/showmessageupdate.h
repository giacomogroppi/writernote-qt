#ifndef SHOWMESSAGEUPDATE_H
#define SHOWMESSAGEUPDATE_H

#include <QDialog>
#include <QString>

namespace Ui {
class ShowMessageUpdate;
}

class ShowMessageUpdate : public QDialog
{
    Q_OBJECT

public:
    explicit ShowMessageUpdate(QWidget *parent = nullptr,
                               QString body = "",
                               QString new_ver = "",
                               bool *c = nullptr);
    ~ShowMessageUpdate();

private slots:
    void on_button_copy_url_clicked();

    void on_pushButton_close_clicked();

    void on_pushButton_terminal_command_clicked();

    void on_pushButton_brow_clicked();

    void on_pushButton_update_now_clicked();

private:
#if defined(WIN32) || defined(WIN64)
    bool *click;
#endif

    Ui::ShowMessageUpdate *ui;
};

#endif // SHOWMESSAGEUPDATE_H
