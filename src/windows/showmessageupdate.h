#ifndef SHOWMESSAGEUPDATE_H
#define SHOWMESSAGEUPDATE_H

#include <QDialog>

namespace Ui {
class ShowMessageUpdate;
}

class ShowMessageUpdate : public QDialog
{
    Q_OBJECT

public:
    explicit ShowMessageUpdate(QWidget *parent = nullptr);
    ~ShowMessageUpdate();

private:
    Ui::ShowMessageUpdate *ui;
};

#endif // SHOWMESSAGEUPDATE_H
