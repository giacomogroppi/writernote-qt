#pragma once


#include <QDialog>

#define URL_ISSUE_GITHUB "https://github.com/giacomogroppi/writernote-qt/issues"
#define ISSUE_MESSAGE "if you want to report a bug you can do it in two ways, send an \nemail to giamg01@gmail.com, or to https://github.com/giacomogroppi/writernote-qt/issues.\notherwise you can create an issue on Github"
#define MAIL "giamg01@gmail.com"

#define NONE ""

namespace Ui {
class bug_repo_ui;
}

class bug_repo_ui : public QDialog
{
    Q_OBJECT

public:
    explicit bug_repo_ui(QWidget *parent = nullptr);
    ~bug_repo_ui();

private slots:
    void on_button_copy_mail_clicked();

    void on_button_open_github_clicked();

private:
    Ui::bug_repo_ui *ui;
};

