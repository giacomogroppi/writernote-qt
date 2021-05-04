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
                               QString new_ver = "");
    ~ShowMessageUpdate();

private slots:
    void on_button_copy_url_clicked();

    void on_pushButton_close_clicked();

    void on_pushButton_terminal_command_clicked();

    void on_pushButton_brow_clicked();

private:
    Ui::ShowMessageUpdate *ui;
};

#endif // SHOWMESSAGEUPDATE_H
