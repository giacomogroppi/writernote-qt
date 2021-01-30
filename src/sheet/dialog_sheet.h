#ifndef DIALOG_SHEET_H
#define DIALOG_SHEET_H

#include <QDialog>

namespace Ui {
class dialog_sheet;
}

class dialog_sheet : public QDialog
{
    Q_OBJECT

public:
    explicit dialog_sheet(QWidget *parent = nullptr);
    ~dialog_sheet();

private:
    Ui::dialog_sheet *ui;
};

#endif // DIALOG_SHEET_H
